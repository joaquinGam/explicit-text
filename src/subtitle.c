#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/subtitle.h"
#include "../include/sub_time.h"
#include "../include/string_utils.h"
#include "../include/str_list.h"
#include "../include/error.h"

void subtitle_init(subtitle_t *subtitle, int index, subtitle_time_t start, subtitle_time_t end, char *text){
  subtitle->index = index;
  subtitle->start = start;
  subtitle->end = end;
  subtitle->text = text;
}

char *subtitle_to_string(subtitle_t subtitle){
  char *text = malloc(600);
  sprintf(text, __SRT_FORMAT, str_by_longint(subtitle.index), \
               subtitle_time_to_string(subtitle.start), subtitle_time_to_string(subtitle.end),\
               subtitle.text);
  text = realloc(text, strlen(text));
  return text;
}

void free_subtitle_string(char ***string){
  free((*string));
  free((*string)+1);
  free((*string)+2);
  free((*string)+3);
  free(string);
}

char **read_subtitle_string(FILE *filein){
  char **str = (char **)malloc(sizeof(char *)*4);
  str[3] = (char *)malloc(400);
  char *temp_str = malloc(100);

  int result = fscanf(filein, __SRT_SHORT_FORMAT, str[0], str[1], str[2]);
  if (result == 3) {
    temp_str = fgets(temp_str, 200, filein);
    str[3][0] = '\0';
    while (!is_string_white(temp_str)){
      str[3] = strcat(str[3],temp_str);
      temp_str = fgets(temp_str, 200, filein);
    }
  } else {
    str = (char **)NULL;
  }
  free(temp_str);
  return str;
}

subtitle_t read_subtitle(FILE *file){
  char **subtitle_string = read_subtitle_string(file);
  subtitle_t subtitle;
  if (subtitle_string){
    subtitle_init(&subtitle, str_to_int(subtitle_string[0]), subtitle_time_by_string(subtitle_string[1]), \
                             subtitle_time_by_string(subtitle_string[2]), subtitle_string[3]);
  } else {
    subtitle.index = -1;
  }
  return subtitle;
}

int delete_subtitle(FILE *filein, FILE **fileout, int index){
  if (index < 1) {
    return 1;
  }
  rewind(filein);
  char **actual_string;
  actual_string = read_subtitle_string(filein);
  if (index != 1) {
    while (actual_string != NULL && str_to_int(actual_string[0]) != index){
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      free_subtitle_string(&actual_string);
      actual_string = read_subtitle_string(filein);
    }
  }
  int return_value = 0;
  if (actual_string != NULL){
    free_subtitle_string(&actual_string);
    actual_string = read_subtitle_string(filein);

    while (actual_string != NULL){
      int actual_index = str_to_int(actual_string[0]) - 1;
      free(actual_string[0]);
      actual_string[0] = str_by_longint((long int)(actual_index));
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      free_subtitle_string(&actual_string);
      actual_string = read_subtitle_string(filein);
    }
    free_subtitle_string(&actual_string);
    return_value = 0;
  } else {
    return_value = 1;
  }
  return return_value;
}

int add_subtitle(FILE *filein, FILE **fileout, subtitle_t subtitle){
  rewind(filein);
  char **actual_string;
  char *last_index = NULL;
  actual_string = read_subtitle_string(filein);
  while (actual_string && subtitle.start > subtitle_time_by_string(actual_string[1])){
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    last_index = actual_string[0];
    free_subtitle_string(&actual_string);
    actual_string = read_subtitle_string(filein);
  }
  int new_index;
  if (!actual_string){
    if (last_index){
      new_index = str_to_int(last_index)+1;
    } else {
      new_index = 1;
    }
  } else {
    new_index = str_to_int(actual_string[0]);
  }
  subtitle.index = new_index;
  fprintf((*fileout), "%s", subtitle_to_string(subtitle));
  if (actual_string){
    do {
      int actual_index = str_to_int(actual_string[0]) + 1;
      actual_string[0] = str_by_longint((long int)(actual_index));
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      free_subtitle_string(&actual_string);
      actual_string = read_subtitle_string(filein);
    } while (actual_string);
  }
  free_subtitle_string(&actual_string);
  return new_index;
}

void verify_subtitle_file(FILE *file){
  rewind(file);

  str_list_t list_of_errors = NULL;
  int actual_index = 1, first_subtitle = 1;
  subtitle_t previous_subtitle, actual_subtitle;
  previous_subtitle = actual_subtitle = read_subtitle(file);
  while (actual_subtitle.index != -1){
    if (first_subtitle){
      if (actual_subtitle.index != actual_index){
        char *tmp_str_err = str_cat(2, "",__SUBTITLE_ERR_FIRST_INDEX);
        str_list_add(&list_of_errors, tmp_str_err);
        actual_index = 0;
      }
      first_subtitle = 0;
    } else {
      if (actual_index && actual_subtitle.index != actual_index){
        char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_LOST_CONSECUTIVE_INDEX, str_by_int(actual_index));
        str_list_add(&list_of_errors, tmp_str_err);
        actual_index = 0;
      }
      if (actual_subtitle.start - previous_subtitle.end < 75){
        char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_SHORT_TIME_BETWEEN_SUBTITLE, str_by_int(actual_subtitle.index));
        str_list_add(&list_of_errors, tmp_str_err);
      }
      if (previous_subtitle.end >= actual_subtitle.start){
        char *tmp_str_err = str_cat(3, __SUBTITLE_ERR_OVERLAPPING_FIRST, str_by_int(actual_subtitle.index), \
                                    __SUBTITLE_ERR_OVERLAPPING_SECOND);
        str_list_add(&list_of_errors, tmp_str_err);
      }
    }
    if (actual_subtitle.end - actual_subtitle.start < 1000){
      char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_SHORT_TIME, str_by_int(actual_subtitle.index));
      str_list_add(&list_of_errors, tmp_str_err);
    } else if (actual_subtitle.end - actual_subtitle.start > 7000){
      char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_LONG_TIME, str_by_int(actual_subtitle.index));
      str_list_add(&list_of_errors, tmp_str_err);
    }
    // the last line in this variable is a white line for de format in the file
    int lines_amount = str_count_lines(actual_subtitle.text)-1;
    if (lines_amount > 2){
      char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_MANY_LINES, str_by_int(actual_subtitle.index));
      str_list_add(&list_of_errors, tmp_str_err);
    }
    int have_many_chars = 0;
    int chars_amount = 0;
    for (int i = 1; i <= lines_amount; i++){
      int chars = str_line_len(i, actual_subtitle.text);
      if (chars > 36)
        have_many_chars = 1;
      chars_amount += chars;
    }
    if (have_many_chars){
      char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_MANY_CHARS, str_by_int(actual_subtitle.index));
      str_list_add(&list_of_errors, tmp_str_err);
    }
    if ((int)(chars_amount/25) > (int)((actual_subtitle.end - actual_subtitle.start)/1000)){
      char *tmp_str_err = str_cat(2, __SUBTITLE_ERR_MANY_CHARS_FOR_SECOND, str_by_int(actual_subtitle.index));
      str_list_add(&list_of_errors, tmp_str_err);
    }

    if (actual_index){
      actual_index+=1;
    }
    previous_subtitle = actual_subtitle;
    actual_subtitle = read_subtitle(file);
  }
  error_list_report(list_of_errors, 0);
  str_list_free(&list_of_errors);
}

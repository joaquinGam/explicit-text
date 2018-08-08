#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/subtitle.h"
#include "../include/sub_time.h"
#include "../include/string_utils.h"

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
}

char **read_subtitle_string(FILE *filein){
  char **str = (char **)malloc(sizeof(char *)*4);
  str[0] = malloc(5); str[1] = malloc(13); str[2] = malloc(13); str[3] = malloc(400);
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
      actual_string[0] = str_by_longint((long int)(actual_index));
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      free_subtitle_string(&actual_string);
      actual_string = read_subtitle_string(filein);
    }
    return_value = 0;
  } else {
    return_value = 1;
  }
  free_subtitle_string(&actual_string);
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

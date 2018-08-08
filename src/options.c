#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/options.h"
#include "../include/error.h"
#include "../include/subtitle.h"
#include "../include/string_utils.h"
#include "../include/str_list.h"

int is_modification_option(char *option){
  if (!strcmp(option,"-b") || !strcmp(option,"-i")){
    return 1;
  } else {
    return 0;
  }
}

int is_processing_option(char *option){
  if (!strcmp(option,"-v") || is_modification_option(option)){
    return 1;
  }else {
    return 0;
  }
}

int is_option(char *option){
  if (!strcmp(option,"-f") || !strcmp(option,"-o") || !strcmp(option,"--help") || is_processing_option(option)) {
    return 1;
  } else {
    return 0;
  }
}

int check_input_output(int argc, char **argv, char **filein, char **fileout){
  int input = 0, output = 0;
  int save_filein = 0, save_fileout = 0;
  int modification_option = 0, processing_option = 0, help = 0, unknown_parameter = 0;
  for (int i = 1; i < argc; i++){
    if (!strcmp(argv[i],"-f")){
      save_filein = 1;
      input += 1;
    } else if (!strcmp(argv[i],"-o")){
      save_fileout = 1;
      output += 1;
    } else if (is_modification_option(argv[i])){
      modification_option += 1;
    } else if (save_filein){
      save_filein = 0;
      *filein = argv[i];
    } else if (save_fileout){
      save_fileout = 0;
      *fileout = argv[i];
    } else if (!strcmp(argv[i], "--help")) {
      help = 1;
      printf("%s\n", __HELP_MESSAGE);
    } else if (is_processing_option(argv[i])) {
      processing_option += 1;
    } else if (argv[i][0] == 45){
      unknown_parameter = 1;
      break;
    }
  }
  if (unknown_parameter){
    return -3;
  } else if (!input && !output && !modification_option && !processing_option){
    if (help){
      return 3;
    } else {
      return -5;
    }
  } else if (input > 1 || output > 1){
    return -1;
  } else if (!input || (modification_option && !output)){
    return -2;
  } else if (output) {
    return 0;
  } else {
    return 1;
  }
}

int save_fileout(FILE *filein, FILE **fileout){
  rewind(filein);
  rewind(*fileout);
  char **actual_string = read_subtitle_string(filein);
  if (actual_string == NULL){
    free(actual_string);
    return 1;
  }
  while (actual_string != NULL){
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    free(actual_string);
    actual_string = read_subtitle_string(filein);
  }
  free(actual_string);
  return 0;
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
        str_list_add(&list_of_errors, __SUBTITLE_ERR_FIRST_INDEX);
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
}

void executeProcessingOptions(FILE *filein, FILE **fileout, int argc, char **argv){
  subtitle_t subtitle;
  subtitle_time_t inf_start_millis, inf_end_millis;
  int delete = 0, startmillis = 0, endmillis = 0, text = 0;
  FILE *tmp_filein = filein;
  FILE *tmp_file = tmpfile();
  FILE *tmp_fileout = tmpfile();
  int changeFile = 0, isFirstOption = 1, processing_options = 0;
  int i = 1;
  for (; i < argc; i++){
    if ((delete || startmillis || endmillis || text) && is_option(argv[i])){
      ERROR_REPORT(6, argv[i-1]);
    }
    if(!strcmp("-b", argv[i])) {
      delete = 1;
    } else if(!strcmp("-i", argv[i])) {
      startmillis = 1;
    } else if(!strcmp("-v", argv[i])) {
      verify_subtitle_file(tmp_filein);
    } else if(delete) {
      if (delete_subtitle(tmp_filein, &tmp_fileout, str_to_int(argv[i]))){
        fprintf(stderr, "(-)-> The index %s is non-existent.\n", argv[i]);
      } else {
        printf("(+)-> The index %s has been removed correctly.\n", argv[i]);
      }
      changeFile = 1;
      delete = 0;
    } else if(startmillis){
      startmillis = 0;
      inf_start_millis = (subtitle_time_t)str_to_int(argv[i]);
      endmillis = 1;
    } else if(endmillis){
      endmillis = 0;
      inf_end_millis = (subtitle_time_t)str_to_int(argv[i]);
      text = 1;
    } else if(text){
      subtitle_init( &subtitle, -1, inf_start_millis, inf_end_millis, argv[i]);
      int index = add_subtitle(tmp_filein, &tmp_fileout, subtitle);
      printf("(+)-> The index %d has been added correctly.\n", index);
      if (isFirstOption){
        changeFile = 1;
        isFirstOption = 0;
      }
      text = 0;
    }
    if (changeFile){
      if (isFirstOption){
        tmp_filein = tmp_fileout;
        tmp_fileout = tmp_file;
        isFirstOption = 0;
      } else {
        FILE *aux = tmp_filein;
        tmp_filein = tmp_fileout;
        tmp_fileout = aux;
      }
      processing_options += 1;
      changeFile = 0;
    }
  }

  // it always change tmp_filein by tmp_fileout, so always tmp_filein has the correct file
  if ((processing_options && save_fileout(tmp_filein, fileout))){
    fprintf(stderr, "(-) Error: An unexpected error has occurred.\n");
  }

  fclose(tmp_filein);
  fclose(tmp_fileout);

  if (delete || startmillis || endmillis || text){
    ERROR_REPORT(6, argv[i-1]);
  }
}

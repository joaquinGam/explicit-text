#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/options.h"
#include "../include/error.h"
#include "../include/subtitle.h"
#include "../include/string_utils.h"

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
    free(temp_str);
    return str;
  } else {
    return (char **)NULL;
  }
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
      actual_string = read_subtitle_string(filein);
    }
  }
  if (actual_string != NULL){
    actual_string = read_subtitle_string(filein);

    while (actual_string != NULL){
      int actual_index = str_to_int(actual_string[0]) - 1;
      actual_string[0] = str_by_longint((long int)(actual_index));
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      actual_string = read_subtitle_string(filein);
    }
    return 0;
  } else {
    return 1;
  }
}

void add_subtitle(FILE *filein, FILE **fileout, subtitle_t subtitle){
  rewind(filein);
  char **actual_string;
  char *last_index = NULL;
  actual_string = read_subtitle_string(filein);

  while (actual_string && subtitle.start > subtitle_time_by_string(actual_string[1])){
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    last_index = actual_string[0];
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
      actual_string = read_subtitle_string(filein);
    } while (actual_string);
  }
}

int save_fileout(FILE *filein, FILE **fileout){
  rewind(filein);
  rewind(*fileout);
  char **actual_string = read_subtitle_string(filein);
  if (actual_string == NULL)
    return 1;
  while (actual_string != NULL){
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    actual_string = read_subtitle_string(filein);
  }
  return 0;
}

extern int error_number;

void executeProcessingOptions(FILE *filein, FILE **fileout, int argc, char **argv){
  subtitle_t subtitle;
  subtitle_time_t inf_start_millis, inf_end_millis;
  int delete = 0, startmillis = 0, endmillis = 0, text = 0;
  FILE *tmp_filein = filein;
  FILE *tmp_file = tmpfile();
  FILE *tmp_fileout = tmpfile();
  int changeFile = 0, isFirstOption = 1, processing_options = 0;
  for (int i = 1; i < argc; i++){
    if ((delete || startmillis || endmillis || text) && is_option(argv[i])){
      error_number = 6;
      error_report();
    } else if(!strcmp("-b", argv[i])) {
      delete = 1;
    } else if(!strcmp("-i", argv[i])) {
      startmillis = 1;
    } else if(!strcmp("-v", argv[i])) {
      // verify_subtitles(file);
    } else if(delete) {
      if (delete_subtitle(tmp_filein, &tmp_fileout, str_to_int(argv[i]))){
        printf("(-) --> The index to be eliminated is non-existent.\n");
      } else {
        printf("(+) --> The index %s has been removed correctly.\n", argv[i]);
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
      add_subtitle(tmp_filein, &tmp_fileout, subtitle);
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
  if (delete || startmillis || endmillis || text){
    error_number = 6;
    error_report();
  }

}

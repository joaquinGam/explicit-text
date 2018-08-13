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

int save_fileout(FILE *filein, FILE **fileout){
  rewind(filein);
  rewind(*fileout);
  char **actual_string; actual_string = read_subtitle_string(filein);
  if (actual_string == NULL){
    return 1;
  }
  while (actual_string != NULL){
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    free_subtitle_string(&actual_string);
    actual_string = read_subtitle_string(filein);
  }
  free_subtitle_string(&actual_string);
  return 0;
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

  fclose(tmp_file);
  fclose(tmp_filein);

  if (delete || startmillis || endmillis || text){
    ERROR_REPORT(6, argv[i-1]);
  }
}

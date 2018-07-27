#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/options.h"
#include "../include/error.h"

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
// -5 - missing parameters ; -3 - unknown parameters ; -2 - missing information ; -1 - a lot of parameters
// 0 - okey ; 1 - input ; 3 - help

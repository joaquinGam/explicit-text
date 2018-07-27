#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/options.h"
#include "../include/error.h"
#include "../include/subtitle.h"
#include "../include/sub_time.h"
#include "../include/string_utils.h"


extern int error_number;


#ifndef NULL // --------------------------------- SACAR DE ACÁ!
  #define NULL 0
#endif
#define __SRT_FORMAT "%s\r\n%s --> %s\r\n%s\r\n\r\n"
#define __SRT_SHORT_FORMAT " %s \r\n %s --> %s \r\n"

char **read_subtitle_string(FILE *filein){
  char **str = (char **)malloc(sizeof(char *)*4);
  str[0] = calloc(5, sizeof(char)); str[1] = calloc(13, sizeof(char));
  str[2] = calloc(13, sizeof(char)); str[3] = calloc(400, sizeof(char));
  /*for (int i = 0; i < 4; i++)
    str[i][0] = '\0';
  */
  char *temp_str = malloc(100);

  int result = fscanf(filein, __SRT_SHORT_FORMAT, str[0], str[1], str[2]);
  temp_str = fgets(temp_str, 200, filein);
  str[3][0] = '\0';
  while (!is_string_white(temp_str)){
    str[3] = strcat(str[3],temp_str);
    temp_str = fgets(temp_str, 200, filein);
  }
  free(temp_str);
  if (result == 3) {
    return str;
  } else {
    return (char **)NULL;
  }
}

int delete_subtitle(FILE *filein, FILE **fileout, int index){
  rewind(filein);
  char **actual_string = read_subtitle_string(filein);

  printf("%s\n", "step 0");
  if (actual_string == 0){
    printf("%s\n", "It's null!");
  }
  while (actual_string != NULL && str_to_int(actual_string[0]) != index){
    printf("%s\n", "step 1");
    fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
    actual_string = read_subtitle_string(filein);
  }
  if (actual_string != NULL){
    printf("%s\n", "step 2");
    actual_string = read_subtitle_string(filein);

    while (actual_string != NULL){
      printf("%s\n", "step 3");
      int actual_index = str_to_int(actual_string[0]) - 1;
      actual_string[0] = str_by_longint((long int)(actual_index));
      fprintf((*fileout), __SRT_FORMAT, actual_string[0], actual_string[1], actual_string[2], actual_string[3]);
      actual_string = read_subtitle_string(filein);
    }
    printf("%s\n", "step 4");
    // fprintf((*fileout), "%c\n", (char)EOF);
    // printf("%s\n", "step 5");
    return 0;
  } else {
    return 1;
  }
}

int main (int argc, char *argv[]){
  char *path_filein = NULL; char *path_fileout = NULL;
  FILE *filein = NULL; FILE *fileout = NULL;
  int io_state = check_input_output(argc, argv, &path_filein, &path_fileout);

  // error reporting
  if (io_state < 0){
    error_number = fabs(io_state);
    error_report();
  } else if (io_state == 5){
    exit(0);
    return 0;
  }

  // open files
  if (path_filein == NULL || (io_state == 0 && path_fileout == NULL)){
    error_number = 2;
    error_report();
  } else {
    filein = fopen(path_filein, "r");
    if (filein == NULL){
      error_number = 4;
      error_report_file(path_filein);
    }
    if (path_fileout != NULL){
      fileout = fopen(path_fileout, "w+");
      if (fileout == NULL){
        error_number = 4;
        error_report_file(path_fileout);
      }
    }
  }

  delete_subtitle(filein, &fileout, 3);

  fclose(filein);
  if (fileout != NULL){
    fclose(fileout);
  }
  printf("%s\n", "Finish step! :D");
  return 0;
}

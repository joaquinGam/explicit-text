#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/options.h"
#include "../include/error.h"
#include "../include/string_utils.h"

extern int error_number;

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

  executeProcessingOptions( filein, &fileout, argc, argv);

  fclose(filein);
  if (fileout != NULL){
    fclose(fileout);
  }
  return 0;
}

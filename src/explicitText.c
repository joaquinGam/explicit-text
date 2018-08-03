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
    ERROR_REPORT(fabs(io_state));
  } else if (io_state == 5){
    exit(0);
    return 0;
  }

  // open files
  if (path_filein == NULL || (io_state == 0 && path_fileout == NULL)){
    ERROR_REPORT(2);
  } else {
    filein = fopen(path_filein, "r");
    if (filein == NULL){
      ERROR_REPORT(4, path_filein);
    }
    if (path_fileout != NULL){
      fileout = fopen(path_fileout, "w+");
      if (fileout == NULL){
        ERROR_REPORT(4, path_fileout);
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

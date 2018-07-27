#include <stdio.h>
#include <stdlib.h>
#include "../include/error.h"

int error_number = 0;

void report(char *message){
  fprintf(stderr, "Error: %s%s\n", message, __LITTLE_HELP);
}

int error_report(){
  int error_identifier = 0;
  switch (error_number){
    case 1:
      report(__MORE_PARAMETERS);
      error_identifier = 1;
      break;
    case 2:
      report(__MISSING_INFORMATION);
      error_identifier = 2;
      break;
    case 3:
      report(__UNKNOWN_PARAMETERS);
      error_identifier = 3;
      break;
    case 5:
      report(__MISSING_PARAMETERS);
      error_identifier = 2;
      break;
  }
  exit(error_identifier);
  return error_identifier;
}

int error_report_file(char *file){
  fprintf(stderr, "Error: %s: %s\n", __INACCESSIBLE_FILE, file);
  exit(4);
  return 4;
}

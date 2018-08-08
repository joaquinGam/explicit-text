#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/error.h"
#include "../include/string_utils.h"
#include "../include/str_list.h"

void report1(char *message){
  fprintf(stderr, "Error: %s %s\n", message, __LITTLE_HELP);
}

void report2(char *message, char *str){
  fprintf(stderr, "Error: %s %s\n", message, str);
}

int error_report1(int32_t error_number){
  int error_identifier = 0;
  switch (error_number){
    case 1:
      REPORT(__MORE_PARAMETERS);
      error_identifier = 1;
      break;
    case 2:
      REPORT(__MISSING_INFORMATION);
      error_identifier = 2;
      break;
    case 3:
      REPORT(__UNKNOWN_PARAMETERS);
      error_identifier = 3;
      break;
    case 5:
      REPORT(__MISSING_PARAMETERS);
      error_identifier = 2;
      break;
  }
  exit(error_identifier);
  return error_identifier;
}

int error_report2(int32_t error_number, char *string){
  int error_identifier = 0;
  switch (error_number){
    case 4:
      REPORT(__INACCESSIBLE_FILE, string);
      error_identifier = 4;
      break;
    case 6:
      REPORT(__MISSING_INFORMATION_IN_PARAMETER, string);
      error_identifier = 5;
      break;
  }
  exit(error_identifier);
  return error_identifier;
}

void error_list_report(str_list_t err_list, int close_program){
  str_list_t tmp_list = err_list;
  while (tmp_list != NULL){
    REPORT(tmp_list->element, "");
    tmp_list = tmp_list->next;
  }
  if (close_program){
    exit(close_program);
  }
}

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void str_free(char *str){
  free(str);
}

int str_to_int(char *str){
  return atoi(str);
}

int is_string_white( const char *s)
{
    unsigned char c;
    while (( c = *s) && (c == '\n' || c == ' ' || c == '\r' ) ) ++s;
    return *s == '\0';
}

char *str_by_longint(long int value){
  char *str = malloc(9);
  sprintf(str, "%ld", value);
  str = realloc(str, strlen(str));
  return str;
}

char *str_by_int(int value){
  char *str = malloc(9);
  sprintf(str, "%d", value);
  str = realloc(str, strlen(str));
  return str;
}

int str_count_lines(char *str){
  int lines = 0;
  for (int i = 0; str[i]; i++){
    if (str[i] == '\n'){
      lines++;
    }
  }
  return lines;
}

int str_line_len(int line, char *str){
  int n_line = 0;
  int i, len = 0;
  for (i = 0; n_line != line && str[i]; i++){
    if (str[i] == '\n'){
      n_line++;
    }
  }
  for (; str[i] && str[i] != '\n'; i++){
    if (str[i] != '\r'){
      len++;
    }
  }
  return len;
}

char *str_cat(int argc, ...){
  va_list argv;
  va_start(argv, argc);
  char *str = malloc(500);
  for (int i = 0; i < argc; i++){
    str = strcat(str, va_arg(argv, char *));
  }
  str = realloc(str, strlen(str));
  va_end(argv);
  return str;
}

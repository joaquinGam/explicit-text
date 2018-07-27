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

char *str_invert(char *str){
  int len = strlen(str);
  char *invert = malloc(len+1);
  for (int i=0, j=len-1; j >= 0; i++, j--){
    invert[i] = str[j];
  }
  invert[len] = '\0';
  return invert;
}

char *str_by_longint(long int value){
  char *str = malloc(255);
  sprintf(str, "%ld", value);
  str = realloc(str, strlen(str));
  return str;
}

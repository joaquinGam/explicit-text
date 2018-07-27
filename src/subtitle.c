#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/subtitle.h"
#include "../include/sub_time.h"
#include "../include/string_utils.h"

void subtitle_init(subtitle_t *subtitle, int index, subtitle_time_t start, subtitle_time_t end, char *text){
  subtitle->index = index;
  subtitle->start = start;
  subtitle->end = end;
  subtitle->text = text;
}

char *subtitle_to_string(subtitle_t subtitle){
  char *text = malloc(600);
  sprintf(text, "%s\r\n%s --> %s\r\n%s\r\n \r\n", str_by_longint(subtitle.index), \
               subtitle_time_to_string(subtitle.start), subtitle_time_to_string(subtitle.end),\
               subtitle.text);
  text = realloc(text, strlen(text));
  return text;
}

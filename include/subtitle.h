#include "sub_time.h"
#ifndef SUBTITLE_H
  typedef struct subtitle {
    int index;
    subtitle_time_t start;
    subtitle_time_t end;
    char *text;
  } subtitle_t;

  #define __SRT_FORMAT "%s\r\n%s --> %s\r\n%s\r\n\r\n"
  #define __SRT_SHORT_FORMAT " %s \r\n %s --> %s \r\n"
  #define SUBTITLE_H 1
#endif

void subtitle_init(subtitle_t *subtitle,  int index, subtitle_time_t time_start, subtitle_time_t time_end, char *text);
char *subtitle_to_string(subtitle_t subtitle);

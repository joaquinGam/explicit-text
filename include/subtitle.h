#include "sub_time.h"
#ifndef SUBTITLE_H

  #define __SRT_FORMAT "%s\r\n%s --> %s\r\n%s\r\n\r\n"
  #define __SRT_SHORT_FORMAT " %s \r\n %s --> %s \r\n"
  #define __SUBTITLE_ERR_FIRST_INDEX "the first subtitle don't have a correct index"
  #define __SUBTITLE_ERR_LOST_CONSECUTIVE_INDEX "the index sequence is broken. Lost index "
  #define __SUBTITLE_ERR_SHORT_TIME_BETWEEN_SUBTITLE "the space between subtitle is short. The last subtitle in this comparison have index "
  #define __SUBTITLE_ERR_OVERLAPPING_FIRST "the time in subtitle with index "
  #define __SUBTITLE_ERR_OVERLAPPING_SECOND " is overlapped by the predecessor"
  #define __SUBTITLE_ERR_SHORT_TIME "the time between the start and the end is short in the subtitle with index "
  #define __SUBTITLE_ERR_LONG_TIME "the time between the start and the end is long in the subtitle with index "
  #define __SUBTITLE_ERR_MANY_LINES "the lines amount are high in subtitle with index "
  #define __SUBTITLE_ERR_MANY_CHARS "the lines have many characters in subtitle with index "
  #define __SUBTITLE_ERR_MANY_CHARS_FOR_SECOND "many chars for second in subtitle with index "


  typedef struct subtitle {
    int index;
    subtitle_time_t start;
    subtitle_time_t end;
    char *text;
  } subtitle_t;

  void subtitle_init(subtitle_t *subtitle,  int index, subtitle_time_t time_start, subtitle_time_t time_end, char *text);
  char *subtitle_to_string(subtitle_t subtitle);
  char **read_subtitle_string(FILE *filein);
  int delete_subtitle(FILE *filein, FILE **fileout, int index);
  int add_subtitle(FILE *filein, FILE **fileout, subtitle_t subtitle);
  subtitle_t read_subtitle(FILE *file);

  #define SUBTITLE_H 1
#endif

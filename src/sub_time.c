#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/sub_time.h"
#include "../include/string.h"

char *subtitle_time_to_string(subtitle_time_t milliseconds){
  char *text = malloc(300);
  sprintf(text, "%ld:%ld:%ld,%ld", milliseconds/3600000, (milliseconds%3600000)/60000, (milliseconds%60000)/1000, milliseconds%1000);
  text = realloc(text, strlen(text));
  return text;
}

subtitle_time_t subtitle_time_by_string(char *subtitle_time_string){
  long int hours, minutes, seconds, milliseconds;
  sscanf(subtitle_time_string, "%ld:%ld:%ld,%ld", &hours, &minutes, &seconds, &milliseconds);
  return (subtitle_time_t)hours*3600000 + minutes*60000 + seconds*1000 + milliseconds;
}

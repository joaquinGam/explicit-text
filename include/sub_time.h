#ifndef SUB_TIME_H
  typedef long int subtitle_time_t;

  char *subtitle_time_to_string(subtitle_time_t subtitleTime);
  subtitle_time_t subtitle_time_by_string(char *subtitle_time_string);

  #define SUB_TIME_H 1
#endif

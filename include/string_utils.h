#ifndef STRING_UTILS_H
  char *str_by_longint(long int value);
  char *str_by_int(int value);
  int is_string_white( const char *s);
  int str_to_int(char *str);
  void str_free(char *str);
  int str_count_lines(char *str);
  int str_line_len(int line, char *str);
  char *str_cat(int argc, ...);

  #define STRING_UTILS_H 1
#endif

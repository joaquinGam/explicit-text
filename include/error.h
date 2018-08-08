#include "str_list.h"
#ifndef ERROR_H
  #define __MISSING_PARAMETERS  "missing parameters."
  #define __MORE_PARAMETERS  "a lot of parameters -f or -o."
  #define __MISSING_INFORMATION "missing information."
  #define __MISSING_INFORMATION_IN_PARAMETER "missing information in parameter "
  #define __UNKNOWN_PARAMETERS "unknown one or more parameters."
  #define __LITTLE_HELP  " Please try: 'explicitText --help' for more information"
  #define __INACCESSIBLE_FILE "inaccessible file"
  #define __HELP_MESSAGE "Use: explicitText [OPTION] information ... \r\n \
                          \r\n Options: \
                          \r\n     --help     \t this message \
                          \r\n     -f filein  \t file to process \
                          \r\n     -o fileout \t output file \
                          \r\n     -b index   \t search and remove the subtitle with this index \
                          \r\n     -i startmilis endmillis text \t insert a subtitle with this text and this time, \
                          \r\n                                  \t and correct the following indexes\
                          \r\n     -v         \t search errors in the subtitles \
                          \r\n -f is obligatory. \
                          \r\n -o is obligatory if any modification operation is requested."

  // this is the magic way to have optional parameters without clarifying how many happened
  #define NARGS(...) NARGS_(__VA_ARGS__, 5, 4, 3, 2, 1, 0)
  #define NARGS_(_5, _4, _3, _2, _1, N, ...) N

  #define CONC(A, B) CONC_(A, B)
  #define CONC_(A, B) A##B

  #define ERROR_REPORT(...) CONC(error_report, NARGS(__VA_ARGS__))(__VA_ARGS__)
  #define REPORT(...) CONC(report, NARGS(__VA_ARGS__))(__VA_ARGS__)

  int error_report1(int32_t error_number);
  int error_report2(int32_t error_number, char *string);
  void error_list_report(str_list_t err_list, int close_program);

  #define ERROR_H
#endif

#ifndef ERROR_H
  #define __MISSING_PARAMETERS  "missing parameters."
  #define __MORE_PARAMETERS  "a lot of parameters -f or -o."
  #define __MISSING_INFORMATION "missing information."
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
                          \r\n -o is obligatory if any modification operation is required."

  #define ERROR_H
#endif

int error_report();
int error_report_file(char *file);

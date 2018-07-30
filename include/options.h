int is_modification_option(char *option);
int is_option(char *option);
int check_input_output(int argc, char **argv, char **filein, char **fileout); // returns:
// -5 - missing parameters ; -3 - unknown parameters ; -2 - missing information ; -1 - a lot of parameters
// 0 - it's all okey ; 1 - input ; 3 - help

int delete_subtitle(FILE *filein, FILE **fileout, int index);
int save_fileout(FILE *file, FILE **fileout);
void executeProcessingOptions(FILE *filein, FILE **fileout, int argc, char **argv);

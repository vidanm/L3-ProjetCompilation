#include "parse_arg.h"

int f_tree = 0;
int f_table = 0;
int f_help = 0;

struct option long_options[] = {
    {"help", no_argument, &f_help, 1},
    {"symtabs", no_argument, &f_table, 1},
    {"tree", no_argument, &f_tree, 1}
};

int parse_arg(int argc, char *argv[]) {
    char *argstring = "hts";
    int opt, option_index;
    while ((opt = getopt_long(argc, argv, argstring, long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h': {
                f_help = 1;
                break;
            }

            case 't': {
                f_tree = 1;
                break;
            }

            case 's': {
                f_table = 1;
                break;
            }

            case 0:{
                // in case of long option
                break;
            }
            default:
                return -1;
                break;
        }
    }
    return 0;
}


void help_reaction(){
    printf("Usage: tpcc [options]\n");
    printf("This program reads from stdin the source file to compile.\n");
    printf("Options:\n");
    printf("\t-h --help   \tDisplay this information.\n");
    printf("\t-t --tree   \tDisplay abstract tree to stdout.\n");
    printf("\t-s --symtabs\tDisplay symbol table to stdout.\n");
}
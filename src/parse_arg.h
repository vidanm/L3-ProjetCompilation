#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

/* flag of program to print AST */
extern int f_tree;
/* flag of program to print symbol table */
extern int f_table;
/* flag to print help */
extern int f_help;

/**
 * Parse program argument.
 * @param argc the argc of main
 * @param argv the argc of main
 *
 * @return in case of success, all global variable defined before will be set
 * proprely, and return 0. If there are invalid options presenting, -1 will be
 * returned.
 */
int parse_arg(int argc, char *argv[]);

/**
 * Print help message to stdout
 */
void help_reaction();
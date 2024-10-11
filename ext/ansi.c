#include<stdio.h>
#include<stdlib.h>
#define ANSI_RESET   "\033[0m"
#define ANSI_BOLD    "\033[1m"
#define ANSI_UNDERLINE "\033[4m"

#define ANSI_BLACK   "\033[30m"
#define ANSI_RED     "\033[31m"
#define ANSI_GREEN   "\033[32m"
#define ANSI_YELLOW  "\033[33m"
#define ANSI_BLUE    "\033[34m"
#define ANSI_MAGENTA "\033[35m"
#define ANSI_CYAN    "\033[36m"
#define ANSI_WHITE   "\033[37m"
#define NO_STYLE     ""

#define FORMAT_STRING_TO_ANSI(color, style, text) (style color text ANSI_RESET)

void print_banner(){
    FILE* bannerf;
    char* banner = malloc(2000*sizeof(char));
    bannerf = fopen("banner.txt", "r");
    int size = fread(banner, sizeof(char), 2000, bannerf );
    banner[size] = '\0';
    fclose(bannerf);
    printf("%s%s%s%s\n\n", ANSI_CYAN, ANSI_BOLD, banner, ANSI_RESET);

}

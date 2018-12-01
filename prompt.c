#include <stdio.h>
#include <stdlib.h>

// If we are compiling on Windows compile these functions
#ifdef _WIN32
#include <string.h>

// Declare a buffer for user input of size 2048
static char buffer[2048];

// Make a fake readline for windows portability
char *readline(char *prompt)
{
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

// Fade add_history for windows
void add_history(char *unused) {}

// Otherwise include the editline headers
#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main(int argc, char **argv)
{

    // Print Version and Exit Information
    puts("Lispy Verison 0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1)
    {
        char *input = readline("lispy> ");
        add_history(input);

        // Echo input back to user
        printf("No you're a %s\n", input);

        free(input);
    }

    return 0;
}
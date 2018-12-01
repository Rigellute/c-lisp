#include <stdio.h>

/* Declare a bugger for user input of size 2048 */
static char input[2048];

int main(int argc, char **argv)
{

    /* Print Version and Exit Information */
    puts("Lispy Verison 0.0.1");
    puts("Press Ctrl+c to Exit\n");

    while (1)
    {

        // Output for prompt
        fputs("lispy> ", stdout);

        // Read a line of user input of maximum size 2048
        fgets(input, 2048, stdin);

        // Echo input back to user
        printf("No you're a %s", input);
    }

    return 0;
}
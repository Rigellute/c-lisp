#include "mpc.h"

#ifdef _WIN32

static char buffer[2048];

char *readline(char *prompt)
{
    fputs(prompt, stdout);
    fgets(buffer, 2048, stdin);
    char *cpy = malloc(strlen(buffer) + 1);
    strcpy(cpy, buffer);
    cpy[strlen(cpy) - 1] = '\0';
    return cpy;
}

void add_history(char *unused) {}

#else
#include <editline/readline.h>
#endif

typedef struct
{
    int type;
    long num;
    int err;
} lval;

// Possible lval types
enum
{
    LVAL_NUM,
    LVAL_ERR
};

// Possible error types
enum
{
    LERR_DIV_ZERO,
    LERR_BAD_OP,
    LERR_BAD_NUM
};

// Create a new number type lval
lval lval_num(long x)
{
    lval v;
    v.type = LVAL_NUM;
    v.num = x;
    return v;
}

// Create a new error type lval
lval lval_err(int x)
{
    lval v;
    v.type = LVAL_ERR;
    v.err = x;
    return v;
}

void lval_print(lval v)
{
    switch (v.type)
    {
    case LVAL_NUM:
        printf("%li", v.num);
        break;
    case LVAL_ERR:
        if (v.err == LERR_DIV_ZERO)
        {
            printf("Error: Division by Zero!");
        }
        if (v.err == LERR_BAD_OP)
        {
            printf("Error: Invalid Operator!");
        }
        if (v.err == LERR_BAD_NUM)
        {
            printf("Error: Invalid Number!");
        }
        break;
    }
}

// Print an "lval" followed by a newline
void lval_println(lval v)
{
    lval_print(v);
    putchar('\n');
}

// Lispy standard library
long min(long x, long y)
{
    return x <= y ? x : y;
}

lval eval_op(lval x, char *op, lval y)
{
    if (x.type == LVAL_ERR)
    {
        return x;
    }
    if (y.type == LVAL_ERR)
    {
        return y;
    }
    if (strcmp(op, "+") == 0)
    {
        return lval_num(x.num + y.num);
    }
    if (strcmp(op, "-") == 0)
    {
        return lval_num(x.num - y.num);
    }
    if (strcmp(op, "*") == 0)
    {
        return lval_num(x.num * y.num);
    }
    if (strcmp(op, "/") == 0)
    {
        return y.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(x.num / y.num);
    }
    if (strcmp(op, "%") == 0)
    {
        return lval_num(x.num % y.num);
    }
    if (strcmp(op, "min") == 0)
    {
        return lval_num(min(x.num, y.num));
    }

    return lval_err(LERR_BAD_OP);
}

lval eval(mpc_ast_t *t)
{
    // If tagged as number return it directly
    if (strstr(t->tag, "number"))
    {
        errno = 0;
        long x = strtol(t->contents, NULL, 10);
        return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
    }

    // The operator is always second child.
    char *op = t->children[1]->contents;
    lval x = eval(t->children[2]);

    int i = 3;
    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }

    return x;
}

int main(int argc, char **argv)
{

    /* Create Some Parsers */
    mpc_parser_t *Number = mpc_new("number");
    mpc_parser_t *Operator = mpc_new("operator");
    mpc_parser_t *Expr = mpc_new("expr");
    mpc_parser_t *Lispy = mpc_new("lispy");

    /* Define them with the following Language */
    mpca_lang(MPCA_LANG_DEFAULT,
              "                                                     \
      number   : /[+-]?([0-9]*[.])?[0-9]+/ ;                             \
      operator : '+' | '-' | '*' | '/' | '%' | \"min\";                  \
      expr     : <number> | '(' <operator> <expr>+ ')' ;  \
      lispy    : /^/ <operator> <expr>+ /$/ ;             \
    ",
              Number, Operator, Expr, Lispy);

    puts("Lispy Version 0.0.0.0.2");
    puts("Press Ctrl+c to Exit\n");

    while (1)
    {

        char *input = readline("lispy> ");
        add_history(input);

        /* Attempt to parse the user input */
        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lispy, &r))
        {
            lval result = eval(r.output);
            lval_println(result);
            mpc_ast_delete(r.output);
        }
        else
        {
            /* Otherwise print and delete the Error */
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    /* Undefine and delete our parsers */
    mpc_cleanup(4, Number, Operator, Expr, Lispy);

    return 0;
}

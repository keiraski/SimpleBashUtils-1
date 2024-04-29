#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <stdlib.h>

typedef struct arguments {
    int b, n, s, E, T, v;
} arguments;
0
arguments argument_parser(int argc, char** argv){
    arguments arg ={0};
    struct option long_options[] = {
        {"number", no_argument, NULL, 'n'},
        {"number-nonblank", no_argument, NULL, 'b'},
        {"squeeze-blank", no_argument, NULL, 's'},
        {0, 0, 0, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "bnEeTts", long_options, 0)) != -1){
        switch(opt){
            case 'b':
            arg.b = 1;
            break;
            case 'n':
            arg.n = 1;
            break;
            case 's':
            arg.s = 1;
            break;
            case 'e':
            arg.E = 1;
            arg.v = 1;
            break;
            case 'E':
            arg.E = 1;
            break;
            case 't':
            arg.T = 1;
            arg.v = 1;
            break;
            case 'T':
            arg.T = 1;
            break;
            case '?':
            perror("ERROR");
            exit(1);
        default:
            break;
        }
    }
    return arg;
}

char v_output(char ch){
    if (ch == '\n' || ch == '\t') return ch;
    if (ch > 0 && ch <= 31){
        putchar('^');
        ch += 64;
    }
    else if (ch == 127) {
        putchar('^');
        ch = '?';
    }
    return ch;
}

void outline(arguments *arg, char* line, int n){
    for (int i = 0; i < n; i++)
    {
        if (arg->T && line[i] == '\t') printf("^I");
        else {
            if (arg->E && line[i] == '\n' && !(arg->b && n == 1)) putchar('$');
            if (arg->v) line[i] = v_output(line[i]);
            putchar(line[i]);
        }
    }
}

void output(arguments* arg, char** argv){
    if (argv[optind] == NULL) {
        fprintf(stderr, "Error: No file name provided\n");
        exit(1);
    }
    FILE* f = fopen(argv[optind], "r");
    if (f == NULL) {
        perror("Error opening file");
        exit(1);
    }
    char* line = NULL;
    size_t memline = 0;
    int read = 0;
    int line_cout = 1;
    int empty_count = 0;
    read = getline(&line, &memline, f);
    while (read != -1) {
        bool is_empty_line = (line[0] == '\n');
        if (is_empty_line) {
            empty_count++;
        } else {
            empty_count = 0;
        }
        if (!(arg->s && empty_count > 1)) {
            if ((arg->n && !arg->b) || (arg->b && !is_empty_line)) {
                printf("%6d\t", line_cout);
            }
            if (!is_empty_line || !arg->b) {
                line_cout++; // Увеличиваем счетчик строк только для непустых строк или если не установлен флаг -b
            }
            outline(arg, line, read);
        }
        read = getline(&line, &memline, f);
    }
    free(line);
    fclose(f);
}

int main(int argc, char** argv){
    arguments arg = argument_parser(argc, argv);
    if (optind >= argc) {
        fprintf(stderr, "Error: No file name provided\n");
        exit(1);
    }
    output(&arg, argv);
    return 0;
}
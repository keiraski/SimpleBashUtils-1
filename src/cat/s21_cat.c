#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct Arguments {
  int b, n, s, E, T, v;
} arguments;

arguments argument_parser(int argc, char** argv) {
  arguments arg = {0, 0, 0, 0, 0, 0};
  struct option long_options[] = {{"number", no_argument, NULL, 'n'},
                                  {"number-nonblank", no_argument, NULL, 'b'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {0, 0, 0, 0}};
  int opt;
  while ((opt = getopt_long(argc, argv, "+bnEeTtsv", long_options, NULL)) !=
         -1) {
    switch (opt) {
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
      case 'v':
        arg.v = 1;
        break;
      case '?':
        fprintf(stderr, "Usage: %s [-bnEeTtsv] [file...]\n", argv[0]);
        exit(1);
    }
  }
  return arg;
}

char v_output(char ch) {
  if (ch == '\n' || ch == '\t') {
    return ch;
  }
  if (ch >= 0 && ch <= 31) {
    putchar('^');
    return ch + 64;
  } else if (ch == 127) {
    putchar('^');
    return '?';
  }
  return ch;
}

void outline(arguments* arg, char* line, ssize_t read, int* line_count,
             int* empty_count) {
  int empty_line = read == 1 && line[0] == '\n';
  if (empty_line) {
    if (arg->s) {
      (*empty_count)++;
      if (*empty_count > 1) {
        return;
      }
    }
  } else {
    *empty_count = 0;
  }
  if ((arg->n && !arg->b) || (arg->b && !empty_line)) {
    printf("%6d\t", (*line_count)++);
  }
  for (int i = 0; i < read; i++) {
    if (arg->v && line[i] != '\n' && line[i] != '\t' &&
        (line[i] < 32 || line[i] == 127)) {
      line[i] = v_output(line[i]);
    } else if (arg->E && line[i] == '\n') {
      if (arg->b && line[0] == '\n') {
        printf("      \t$");
      } else
        putchar('$');
    } else if (arg->T && line[i] == '\t') {
      printf("^I");
      continue;
    }
    putchar(line[i]);
  }
}

void output(arguments* arg, FILE* f) {
  char* line = NULL;
  size_t memline = 0;
  ssize_t read = 0;
  int line_count = 1;
  int empty_count = 0;

  while ((read = getline(&line, &memline, f)) != -1) {
    outline(arg, line, read, &line_count, &empty_count);
  }

  free(line);
}

int main(int argc, char** argv) {
  arguments arg = argument_parser(argc, argv);
  if (optind == argc) {
    output(&arg, stdin);
  } else {
    for (int i = optind; i < argc; i++) {
      FILE* f = fopen(argv[i], "r");
      if (!f) {
        fprintf(stderr, "cat: ");
        perror(argv[i]);
        continue;
      }
      output(&arg, f);
      fclose(f);
    }
  }
  return 0;
}
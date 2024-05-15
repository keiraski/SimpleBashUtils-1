#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char *pattern;
  int len_pattern;
  int mem_pattern;
} arguments;

void pattern_add(arguments *arg, char *pattern) {
  int n = strlen(pattern);
  if (arg->len_pattern == 0) {
    arg->pattern = malloc(1024 * sizeof(char));
    arg->mem_pattern = 1024;
    if (arg->pattern != NULL) {
      arg->pattern[0] = '\0';
    } else {
      exit(1);
    }
  }
  while (arg->mem_pattern < arg->len_pattern + n + 1) {
    char *new_pattern = realloc(arg->pattern, arg->mem_pattern * 2);
    if (new_pattern == NULL) {
      free(arg->pattern);
      exit(1);
    }
    arg->pattern = new_pattern;
    arg->mem_pattern *= 2;
  }
  if (arg->len_pattern != 0) {
    strcat(arg->pattern, "|");
    arg->len_pattern++;
  }
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void add_pat_from_file(arguments *arg, char *filepath) {
  FILE *f = fopen(filepath, "r");
  if (f == NULL) {
    if (!arg->s) perror(filepath);
    exit(1);
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = getline(&line, &memlen, f);
  while (read != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    pattern_add(arg, line);
    read = getline(&line, &memlen, f);
  }
  free(line);
  fclose(f);
}

arguments argument_parser(int argc, char *argv[]) {
  arguments arg = {0};
  int opt = 0;
  for (; opt != -1; opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) {
    switch (opt) {
      case 'e':
        arg.e = 1;
        pattern_add(&arg, optarg);
        break;
      case 'i':
        arg.i = REG_ICASE;
        break;
      case 'v':
        arg.v = 1;
        break;
      case 'c':
        arg.c = 1;
        break;
      case 'l':
        arg.l = 1;
        break;
      case 'n':
        arg.n = 1;
        break;
      case 'h':
        arg.h = 1;
        break;
      case 's':
        arg.s = 1;
        break;
      case 'f':
        arg.f = 1;
        add_pat_from_file(&arg, optarg);
        break;
      case 'o':
        arg.o = 1;
        break;
      case '?':
        exit(1);
        break;
    }
  }
  if (arg.len_pattern == 0 && optind < argc) {  //&& optind < argc
    pattern_add(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind == 1) arg.h = 1;
  return arg;
}

void output_line(char *line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n') putchar('\n');
}

void print_match(regex_t *re, char *line, char *path, int line_count,
                 arguments arg) {
  regmatch_t math;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &math, 0);
    if (result != 0) {
      break;
    } else if (result == REG_NOMATCH)
      break;
    else if (!arg.h)
      printf("%s:", path);
    if (arg.n) printf("%d:", line_count);
    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar((line + offset)[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
}

void processFile(arguments arg, char *path, regex_t *reg) {
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    if (!arg.s) perror(path);
    return;
  }
  char *line = NULL;
  size_t memlen = 0;
  int read = 0;
  int line_count = 1;
  int c = 0;
  read = getline(&line, &memlen, f);
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
      if (!arg.c && !arg.l) {
        if (!arg.h && !arg.o) printf("%s:", path);
        if (arg.n && !arg.o) printf("%d:", line_count);
        if (arg.o) {
          if (result == 0) print_match(reg, line, path, line_count, arg);
          if (result != 0 && !arg.h) printf("%s:", path);
          if (result != 0 && arg.n) printf("%d:", line_count);
          if (result != 0) output_line(line, read);
        } else
          output_line(line, read);
      }
      c++;
    }
    read = getline(&line, &memlen, f);
    line_count++;
  }
  free(line);
  if (arg.c && arg.l) {
    if (!arg.h) printf("%s:", path);
    if (c >= 1)
      printf("1\n");
    else
      printf("0\n");
  }
  if (arg.c && !arg.l) {
    if (!arg.h) printf("%s:", path);
    printf("%d\n", c);
  }
  if (arg.l && c > 0) printf("%s\n", path);
  fclose(f);
}

void output(arguments arg, int argc, char **argv) {
  regex_t re;
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.i);
  if (error) perror("Error");
  for (int i = optind; i < argc; i++) {
    processFile(arg, argv[i], &re);
  }
  regfree(&re);
}

int main(int argc, char **argv) {
  arguments arg = argument_parser(argc, argv);
  output(arg, argc, argv);
  free(arg.pattern);
  return 0;
}
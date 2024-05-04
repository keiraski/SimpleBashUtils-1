#include <getopt.h>
#include <regex.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char* pattern;
  int len_pattern;
  int mem_pattern;

} arguments;

void pattern_add(arguments* arg, char* pattern) {
  int n = strlen(pattern);
  if (arg->len_pattern == 0) {
    arg->pattern = malloc(1024 * sizeof(char));
    arg->mem_pattern = 1024;
    if (arg->pattern != NULL) {
      arg->pattern[0] = '\0';
    } else {
      exit(EXIT_FAILURE);
    }
  }

  while (arg->mem_pattern < arg->len_pattern + n + 1) {
    char* new_pattern = realloc(arg->pattern, arg->mem_pattern * 2);
    if (new_pattern == NULL) {
      // Обработка ошибки выделения памяти
      free(arg->pattern);  // Освобождаем исходный блок памяти
      exit(EXIT_FAILURE);
    }
    arg->pattern = new_pattern;
    arg->mem_pattern *= 2;
  }

  if (arg->len_pattern != 0) {
    strcat(arg->pattern, "|");  // убрать часть с + arg->len_pattern  было 1
                                // аргумент arg->pattern + arg->len_pattern
    arg->len_pattern++;
  }
  arg->len_pattern += sprintf(arg->pattern + arg->len_pattern, "(%s)", pattern);
}

void add_reg_from_file(arguments* arg, char* filepath) {
  FILE* f = fopen(filepath, "r");
  if (f == NULL) {
    if (!arg->s) perror(filepath);
    exit(1);
  }
  char* line = NULL;
  size_t memline = 0;
  ssize_t read =
      getline(&line, &memline, f);  // ssize_t эквивалентен применениею int
  while (read != -1) {
    if (line[read - 1] == '\n') line[read - 1] = '\0';
    pattern_add(arg, line);
    free(line);
    line = NULL;
    read = getline(&line, &memline, f);
  }
  free(line);
  fclose(f);
}

arguments arguments_parser(int argc, char** argv) {
  arguments arg = {0};
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
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
      case 'l':
        arg.c = 1;
        arg.l = 1;
        break;
      case 'c':
        arg.c = 1;
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
        add_reg_from_file(&arg, optarg);
        break;
      case 'o':
        arg.o = 1;
        break;
      default:
        break;
    }
  }
  if (arg.len_pattern == 0 &&
      optind < argc) {  // Проверяем, что optind не выходит за пределы argc
    pattern_add(&arg, argv[optind]);
    optind++;
  }
  if (argc - optind <= 1) {  // Если после обработки опций остался один аргумент
                             // или нет аргументов
    arg.h = 1;
  }
  return arg;
}

void output_line(char* line, int n) {
  for (int i = 0; i < n; i++) {
    putchar(line[i]);
  }
  if (line[n - 1] != '\n')
    putchar('\n');  // если в конце файла нет символка конца строки то ставится
                    // символ конца строки
}

void print_match(regex_t* re, char* line) {
  regmatch_t math;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &math, 0);
    if (result != 0) {
      break;
    }
    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar(line[i]);
    }
    putchar('\n');
    offset += math.rm_eo;
  }
  //   regfree(re);
}

void processFile(arguments arg, char* path, regex_t* reg) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    if (!arg.s) perror(path);
    exit(1);
  }
  char* line = NULL;
  size_t memline = 0;
  int read = 0;
  int line_count = 1;
  int c = 0;
  read = getline(&line, &memline, f);
  // ?
  while (read != -1) {
    int result = regexec(reg, line, 0, NULL, 0);
    if ((result == 0 && !arg.v) || (arg.v && result != 0)) {
      if (!arg.c && !arg.l) {
        if (!arg.h)
          printf("%s:", path);  // для вывода названий файлов при фалге -ci
        if (arg.n) printf("%d:", line_count);  // работает при флаге -n
        if (arg.o) {
          print_match(reg, line);
        } else {
          output_line(line, read);
        }
      }
      c++;  //последняя вложенность работает в случае отсутсвия флага -с
    }
    read = getline(&line, &memline, f);
    line_count++;
  }
  free(line);
  if (arg.c && !arg.l) {
    if (!arg.h)
      printf("%s:", path);  // для вывода названий файлов при фалге -ci
    printf("%d\n", c);
  }
  if (arg.l && c > 0) printf("%s\n", path);
  fclose(f);
  // regfree(reg);
}

void output(arguments arg, int argc, char** argv) {  //выход из всех файлов
  regex_t re;
  // printf("%s\n", arg.pattern);
  int error = regcomp(&re, arg.pattern, REG_EXTENDED | arg.i);
  if (error) perror("Error");
  for (int i = optind; i < argc; i++) {
    processFile(arg, argv[i], &re);
  }
  regfree(&re);
}
// -i
int main(int argc, char** argv) {
  arguments arg = arguments_parser(argc, argv);
  output(arg, argc, argv);
  free(arg.pattern);

  return 0;
}
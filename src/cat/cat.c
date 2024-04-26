#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void flags_parser(char *flags, int argc, char **argv, int *index);
void append_flags(char *flags, char flag);
void append_flag(char *flags, char flag);
int print_file(char *name, char *flags);
void print_symb(int c, int *prev, char *flags, int *index, bool *eline_printed);

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Usage: %s <filename>n", argv[0]);
    return 1;
  }
  char flags[7] = "0";  // массив флагов
  int index_end_flags = 1;
  flags_parser(flags, argc, argv, &index_end_flags);

  int err_code = 0;
  // Перебираем все аргументы после флагов, считая их именами файлов
  for (int i = index_end_flags; i < argc; i++) {
    // Печатаем содержимое каждого файла
    if (print_file(argv[i], flags) != 0) {
      fprintf(stderr, "Error printing file: %sn", argv[i]);
      err_code = 1;
    }
  }
  return err_code;
}

void flags_parser(char *flags, int argc, char **argv, int *index) {
  // Пройдемся по всем аргументам, кроме первого (имени программы)
  for (int i = 1; i < argc; i++) {
    // Если аргумент начинается не на '-' или является строкой "-" или "--"
    // (сигнал, что опции кончились), считаем, что флаги опций кончились
    if (argv[i][0] != '-' || strcmp(argv[i], "--") == 0 ||
        strcmp(argv[i], "-") == 0) {
      *index = i;
      return;  // Выходим из функции, так как флаги закончились
    } else {
      // Флаги могут быть написаны слитно, например: -bE
      // Поэтому проходимся по каждому символу аргумента, кроме первого
      for (size_t j = 1; j < strlen(argv[i]); j++) {
        append_flags(flags, argv[i][j]);
      }
    }
  }
  *index =
      argc;  // Если флаги не найдены, устанавливаем index в конец массива argv
}

struct s_avi_flags {
  char flag;               // флаг
  char *equivalent_flags;  // эквивалентные простые флаги
};

void append_flags(char *flags, char flag) {
  // доступные флаги
  struct s_avi_flags avi_flags[8] = {{'b', "b"},  {'E', "E"}, {'e', "Ev"},
                                     {'n', "n"},  {'s', "s"}, {'T', "T"},
                                     {'t', "Tv"}, {'v', "v"}};

  for (int i = 0; i < 8; i++) {
    if (avi_flags[i].flag == flag) {
      for (size_t j = 0; j < strlen(avi_flags[i].equivalent_flags); j++) {
        append_flag(flags, avi_flags[i].equivalent_flags[j]);
      }
      break;
    }
  }
}

// добавить 1 флаг в массив флагов
void append_flag(char *flags, char flag) {
  if (strchr(flags, flag) == NULL) {  // если такого флага нет
    size_t len = strlen(flags);
    flags[len] = flag;  // добавляем флаг
    flags[len + 1] = '\0';  // обновляем нулевой терминатор
  }
}

int print_file(char *name, char *flags) {
  int err_code = 0;
  FILE *f =
      fopen(name, "rt");  // Попытка открыть файл для чтения в текстовом режиме

  if (f != NULL) {
    int index = 0;
    bool eline_printed = false;
    int c = fgetc(f), prev = '\n';
    while (c != EOF) {
      print_symb(c, &prev, flags, &index, &eline_printed);
      c = fgetc(f);
    }
    fclose(f);
  } else {
    err_code = 1;
    printf("Файла не существует");
  }
  return err_code;
}

void print_symb(int c, int *prev, char *flags, int *index,
                bool *eline_printed) {
  // если s и это пустая строка и пустая строка уже была выведена, пропустим,
  // сюда не зайдем
  if (!(strchr(flags, 's') != NULL && *prev == '\n' && c == '\n' &&
        *eline_printed)) {
    if (*prev == '\n' && c == '\n')
      *eline_printed = true;
    else
      *eline_printed = false;

    // если ( (n без b) или (b и текущий символ не '\n') ) и пред символ '\n'
    if (((strchr(flags, 'n') != NULL && strchr(flags, 'b') == NULL) ||
         (strchr(flags, 'b') != NULL && c != '\n')) &&
        *prev == '\n') {
      *index += 1;
      printf("%6d\t", *index);
    }

    if (strchr(flags, 'E') != NULL && c == '\n') printf("$");
    if (strchr(flags, 'T') != NULL && c == '\t') {
      printf("^");
      c = '\t' + 64;
    }
    if (strchr(flags, 'v') != NULL && c >= 0 && c <= 31 && c != '\n' &&
        c != '\t') {
      printf("^");
      c = c + 64;
    }
    fputc(c, stdout);
  }
  *prev = c;
}

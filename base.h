#ifndef BASE_H
#define BASE_H


#define OPEN_FILE(FILE, NAME)                           \
  if (argc < 2)                                         \
    {                                                   \
  fprintf(stderr, "Need an input file...\n");           \
  exit(EXIT_FAILURE);                                   \
    }                                                   \
                                                        \
  FILE = fopen(NAME, "r");                              \
  if (!FILE)                                            \
    {                                                   \
  fprintf(stderr, "Can't open the file...\n");          \
  exit(EXIT_FAILURE);                                   \
    }                                                   


#define CLOSE_FILE(FILE)                                \
  if (fclose(FILE))                                     \
    {                                                   \
      fprintf(stderr, "Problem for closing file...\n"); \
      exit(EXIT_FAILURE);                               \
    }


#define SIZE(X) (sizeof(X) / sizeof(X[0]))

#endif /* BASE_H */

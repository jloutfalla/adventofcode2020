/* Basic macro definitions
   Copyright (C) 2020 Jean-Baptiste Loutfalla

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>. */

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

#define MAX_LINE 256

#endif /* BASE_H */

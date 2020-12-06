/* AdventOfCode 2020, Day 5 -- boarding seats
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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "../base.h"

#define ROWS 128
#define COLS 8
#define LINE 10

int compute_pos(int min, int max, char min_char, char **str);
int find_seat_id(char *str);
int find_max_seat_id(FILE *file);
int find_my_seat(FILE *file, int max_id);

int
main(int argc, char *argv[])
{
  FILE *file;
  int num;

  OPEN_FILE(file, argv[1]);

  num = find_max_seat_id(file);
  printf("Num: %d\n", num);

  fseek(file, 0, SEEK_SET);

  num = find_my_seat(file, num);
  printf("My seat: %d\n", num);
  
  CLOSE_FILE(file);
  
  return 0;
}


inline int
compute_pos(int min, int max, char min_char, char **str)
{
  int val;

  while (min != max)
    {
      val = (max - min) / 2;

      if (**str == min_char)
        max = min + val;
      else
        min = max - val;
      
      (*str)++;
    }

  return min;
}


int
find_seat_id(char *str)
{  
  int row, col;

  if (*str == '\0')
    return 0;
  
  row = compute_pos(0, ROWS - 1, 'F', &str);
  col = compute_pos(0, COLS - 1, 'L', &str);

  return row * COLS + col;
}


int
find_max_seat_id(FILE *file)
{
  int tmp;
  int max = -1;
  char str[LINE + 2];

  while (feof(file) == 0)
    {
      fgets(str, SIZE(str), file);
      
      tmp = find_seat_id(str);
      
      if (max < tmp)
        max = tmp;

      str[0] = '\0';
    }
  
  return max;
}


int compare_int(const void *a, const void *b)
{
  int lhs = *(int *) a;
  int rhs = *(int *) b;

  if (lhs == rhs)
    return 0;
  else if (lhs < rhs)
    return -1;
  else
    return 1;
}

int
find_my_seat(FILE *file, int max_id)
{
  int *tab;
  char str[LINE + 2];
  int i;

  tab = (int *) calloc(max_id, sizeof(int));
  if (tab == NULL)
    {
      fprintf(stderr, "Can't allocate enough memory...\n");
      exit(EXIT_FAILURE);
    }

  i = 0;
  while (feof(file) == 0)
    {
      fgets(str, SIZE(str), file);

      tab[i] = find_seat_id(str);
      
      i++;
      str[0] = '\0';
    }

  qsort(tab, max_id, sizeof(int), compare_int);

  for (i = 0; i < max_id - 1; ++i)
    {
      if (tab[i] == 0)
        continue;
      
      if (tab[i] != tab[i + 1] && tab[i] + 1 != tab[i + 1])
        break;
    }

  i = tab[i];
  free(tab);
  
  return i + 1;
}

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

int find_seat_id(const char *str);
int find_max_seat_id(FILE *file);

int
main(int argc, char *argv[])
{
  FILE *file;
  int num;

  OPEN_FILE(file, argv[1]);

  num = find_max_seat_id(file);
  printf("Num: %d\n", num);
  
  CLOSE_FILE(file);
  
  return 0;
}


int
find_seat_id(const char *str)
{  
  int min_row = 0, max_row = ROWS - 1;
  int min_col = 0, max_col = COLS - 1;

  int val;

  if (*str == '\0')
    return 0;
  
  while (min_row != max_row)
    {
      val = (max_row - min_row) / 2;
      
      if (*str == 'F')
        max_row = min_row + val;
      else
        min_row = max_row - val;

      str++;
    }

  while (min_col != max_col)
    {
      val = (max_col - min_col) / 2;

      if (*str == 'L')
        max_col = min_col + val;
      else
        min_col = max_col - val;

      str++;
    }

  return min_row * COLS + min_col;
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

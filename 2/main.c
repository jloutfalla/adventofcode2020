/* AdventOfCode 2020, Day 2 -- verify passwords validity
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


#include "../base.h"


#define MAX_LINE 256

int number_correct(FILE *file);
int number_correct2(FILE *file);

int
main(int argc, char *argv[])
{
  FILE *file;
  int num;

  OPEN_FILE(file, argv[1]);

  num = number_correct(file);
  printf("Num correct: %d\n", num);

  fseek(file, 0, SEEK_SET);
  num = number_correct2(file);
  printf("Num correct 2: %d\n", num);
  
  CLOSE_FILE(file);
  
  
  return 0;
}


int number_correct(FILE *file)
{
  char str[MAX_LINE];
  char *s;
  int num = 0;
  
  int min = -1, max = -1;
  char c;

  int count;
  while (feof(file) == 0)
    {
      fscanf(file, "%d-%d %c: ", &min, &max, &c);
      fgets(str, MAX_LINE, file);
      
      s = str;
      count = 0;
      while (*s)
        {
          if (*s == c)
            count++;

          s++;
        }

      if (count >= min && count <= max)
        num++;

      min = -1;
      max = -1;
    }

  return num;
}


int number_correct2(FILE *file)
{
  char str[MAX_LINE];
  int num = 0;
  
  int min = -1, max = -1;
  char c;

  while (feof(file) == 0)
    {
      fscanf(file, "%d-%d %c: ", &min, &max, &c);
      fgets(str, MAX_LINE, file);

      if (str[min - 1] != str[max - 1] && (str[min - 1] == c || str[max - 1] == c))
        num++;

      min = -1;
      max = -1;
    }

  return num;  
}

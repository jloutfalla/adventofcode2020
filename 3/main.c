/* AdventOfCode 2020, Day 3 -- count the number of trees on a slope
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
#include <string.h>


#include "../base.h"
  
#define MAX_LINE 256

struct slope
{
  int x;
  int y;
};

static struct slope slopes[] = {
  { 1, 1 },
  { 3, 1 },
  { 5, 1 },
  { 7, 1 },
  { 1, 2 },
};

static char tree = '#';

int count_tree_slope(FILE *file, int offsetx, int offsety, char tree);

int
main(int argc, char *argv[])
{
  FILE *file;
  unsigned int i;
  
  double total = 1.0;
  
  OPEN_FILE(file, argv[1]);

  for (i = 0; i < SIZE(slopes); ++i)
    {
      total *= count_tree_slope(file, slopes[i].x, slopes[i].y, tree);
      fseek(file, 0, SEEK_SET);
    }

  printf("Final : %.0f\n", total);
  
  CLOSE_FILE(file);
  
  return 0;
}

int count_tree_slope(FILE *file, int offsetx, int offsety, char tree)
{
  int length;
  int num = 0;
  int pos = 0, line = 0;
  char str[MAX_LINE];

  fgets(str, MAX_LINE, file);
  length = strlen(str) - 1;

  while (feof(file) == 0)
    {
      line++;
      fgets(str, MAX_LINE, file);

      if (line % offsety == 0)
        {
          pos = (pos + offsetx) % length;
          
          if (*str && str[pos] == tree)
            num++;
        }
      
      str[0] = '\0';
    }
  

  return num;
}

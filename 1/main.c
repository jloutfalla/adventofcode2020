/* AdventOfCode 2020, Day 1 -- find the NUM entries that sum to VAL
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

#define VAL 2020
#define NUM 3

void fill_tab(FILE* f, int** tab, int *size);
void find_numbers(int **tab, int size, int *nums);

int
main(int argc, char *argv[])
{
  FILE *file;
  int *tab = NULL;
  int nums[NUM] = { 0 };
  int i, size = 10;

  double total = 1.0;
  
  OPEN_FILE(file, argv[1]);

  tab = calloc(sizeof(int), size);

  fill_tab(file, &tab, &size);

  CLOSE_FILE(file);

  find_numbers(&tab, size, nums);
  
  free(tab);

  for (i = 0; i < NUM; ++i)
    {
      printf("%d\n", nums[i]);
      total *= nums[i];
    }

  printf("Total: %.0f\n", total);
  
  return 0;
}


void
fill_tab(FILE* f, int** tab, int* size)
{
  int i;

  i = 0;
  do
    {
      if (i == *size)
        {
          *size = i + 1;
          *tab = (int*) realloc(*tab, *size * sizeof(int));
        }
      
      fscanf(f, "%d", (*tab) + i);
      fgetc(f);

      ++i;
    } while (feof(f) == 0);
}


void
find_numbers(int **tab, int size, int *nums)
{
  int i, j, k;
  int *t = *tab;

  for (i = 0; i < size - 2; ++i)
    for (j = i; j < size - 1; ++j)
      for (k = j; k < size; ++k) 
      {
        if (t[i] + t[j] + t[k] == VAL)
          {
            nums[0] = t[i];
            nums[1] = t[j];
            nums[2] = t[k];
            return;
          }
      }
}

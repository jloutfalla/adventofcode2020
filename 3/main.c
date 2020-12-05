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

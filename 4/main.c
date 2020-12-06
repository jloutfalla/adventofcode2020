/* AdventOfCode 2020, Day 4 -- verify passport data
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
#include <stdint.h>
#include <ctype.h>
#include <errno.h>

#include "../base.h"

#define PASSPORT_BYR 0x1
#define PASSPORT_IYR 0x2
#define PASSPORT_EYR 0x4
#define PASSPORT_HGT 0x8
#define PASSPORT_HCL 0x10
#define PASSPORT_ECL 0x20
#define PASSPORT_PID 0x40
#define PASSPORT_CID 0x80

#define PASSPORT_OK 0x7F
#define PASSPORT_FULL 0xFF

enum
{
  COND_NONE,
  COND_RANGE,
  COND_HEIGHT,
  COND_COLOR,
  COND_STR_RANGE,
  COND_PID,
};

/* Range conditions */
static const int birth_years[2]      = { 1920, 2002 };
static const int issue_years[2]      = { 2010, 2020 };
static const int expiration_years[2] = { 2020, 2030 };

/* Height condition */
static const int heights[2][2] = {
  { 150, 193 },
  { 59,  76  }
};

/* String range condition */
static const char str_range[7][3] = {
  "amb",
  "blu",
  "brn",
  "gry",
  "grn",
  "hzl",
  "oth"
};

/* Color condition */
static const unsigned int color = 6;

/* PID condition */
static const unsigned int pid = 9;

typedef struct
{
  uint8_t type;
  char code[3];
  int condition;
  const void *cond_val; 
} passport_field;

static const passport_field pf[] = {
  { PASSPORT_BYR, "byr", COND_RANGE,     birth_years      },
  { PASSPORT_IYR, "iyr", COND_RANGE,     issue_years      },
  { PASSPORT_EYR, "eyr", COND_RANGE,     expiration_years },
  { PASSPORT_HGT, "hgt", COND_HEIGHT,    NULL             },
  { PASSPORT_HCL, "hcl", COND_COLOR,     &color           },
  { PASSPORT_ECL, "ecl", COND_STR_RANGE, NULL             },
  { PASSPORT_PID, "pid", COND_PID,       &pid             },
  { PASSPORT_CID, "cid", COND_NONE,      NULL             },
};

int cond_range(const char *str, const int *range);
int cond_str_range(const char *str);
int cond_height(const char *str);
int cond_color(const char *str, unsigned int num);
int cond_pid(const char *str, unsigned int num);


int num_passport_correct_v1(FILE *file);
int num_passport_correct_v2(FILE *file);

int
main(int argc, char *argv[])
{
  FILE *file;
  int num;
  
  OPEN_FILE(file, argv[1]);

  num = num_passport_correct_v1(file);
  printf("Num v1: %d\n", num);

  fseek(file, 0, SEEK_SET);
  
  num = num_passport_correct_v2(file);
  printf("Num v2: %d\n", num);
  
  CLOSE_FILE(file);
  
  return 0;
}


int
num_passport_correct_v1(FILE *file)
{
  char str[MAX_LINE];
  char *cpy, *tmp1, *tmp2;
  char *token, *subtoken;
  char *saveptr1, *saveptr2;
  uint8_t passport = 0x0;
  unsigned int i;
  
  int num = 0;

  cpy = NULL;
  while (feof(file) == 0)
    {
      fgets(str, MAX_LINE, file);

      if (cpy != NULL)
        free(cpy);
      
      if (*str == '\n' || *str == '\0')
        {
          if ((passport & PASSPORT_OK) == PASSPORT_OK)
            num++;

          passport = 0x0;
          cpy = NULL;
          continue;
        }

      cpy = strdup(str);
      for (tmp1 = cpy, token = NULL; ; tmp1 = NULL)
        {
          token = strtok_r(tmp1, " ", &saveptr1);
          if (token == NULL)
            break;
          
          for (tmp2 = token; ; tmp2 = NULL)
            {
              subtoken = strtok_r(tmp2, ":", &saveptr2);
              if (subtoken == NULL)
                break;
              
              for (i = 0; i < SIZE(pf); ++i)
                if (strncmp(subtoken, pf[i].code, 3) == 0)
                  {
                    passport |= pf[i].type;
                    break;
                  }
            }
        }
      
      str[0] = '\0';
    }

  return num;
}


int
num_passport_correct_v2(FILE *file)
{
  char str[MAX_LINE];
  char *cpy, *tmp1, *tmp2;
  char *token, *subtoken;
  char *saveptr1, *saveptr2;
  uint8_t passport = 0x0;
  int cond;
  unsigned int i;
    
  int num = 0;

  cpy = NULL;
  while (feof(file) == 0)
    {
      fgets(str, MAX_LINE, file);

      if (cpy != NULL)
        free(cpy);
      
      if (*str == '\n' || *str == '\0')
        {
          if ((passport & PASSPORT_OK) == PASSPORT_OK)
            num++;

          passport = 0x0;
          cpy = NULL;
          continue;
        }

      cpy = strdup(str);
      for (tmp1 = cpy, token = NULL; ; tmp1 = NULL)
        {
          token = strtok_r(tmp1, " ", &saveptr1);
          if (token == NULL)
            break;
          
          for (tmp2 = token; ; tmp2 = NULL)
            {
              subtoken = strtok_r(tmp2, ":", &saveptr2);
              if (subtoken == NULL)
                break;

              for (i = 0; i < SIZE(pf); ++i)
                if (strncmp(subtoken, pf[i].code, 3) == 0)
                  {
                    subtoken = strtok_r(NULL, ":\n", &saveptr2);
                    cond = 0;

                    switch (pf[i].condition)
                      {
                      case COND_RANGE:
                        cond = cond_range(subtoken, (int *) pf[i].cond_val);
                        break;

                      case COND_HEIGHT:
                        cond = cond_height(subtoken);
                        break;

                      case COND_COLOR:
                        cond = cond_color(subtoken, *((unsigned int*) pf[i].cond_val));
                        break;

                      case COND_STR_RANGE:
                        cond = cond_str_range(subtoken);                        
                        break;

                      case COND_PID:
                        cond = cond_pid(subtoken, *((unsigned int*) pf[i].cond_val));
                        break;

                      case COND_NONE:
                        cond = 1;
                        break;
                      }

                    if (cond != 0)
                      passport |= pf[i].type;
                    break;
                  }
            }
        }
         
      str[0] = '\0';
    }

  return num;
}


inline int
cond_range(const char *str, const int *range)
{
  char *end;
  long int val;

  errno = 0;
  val = strtol(str, &end, 0);

  if (errno == EINVAL || errno == ERANGE)
    return 0;
  
  return (range[0] <= val && val <= range[1]);
}

  
inline int
cond_str_range(const char *str)
{
  size_t i;

  for (i = 0; i < SIZE(str_range); ++i)
    if (strncmp(str, str_range[i], SIZE(str_range[i])) == 0)
      return 1;
  
  return 0;
}


inline int
cond_height(const char *str)
{
  char *end;
  long int height;

  height = strtol(str, &end, 0);
  
  if (strncmp(end, "cm", 2) == 0)
    return (heights[0][0] <= height && height <= heights[0][1]);
  
  return (heights[1][0] <= height && height <= heights[1][1]);
}


inline int
cond_color(const char *str, unsigned int num)
{
  unsigned int i;

  if (*str != '#' || strlen(str) != num + 1)
    return 0;

  str++;

  for (i = 0; i < num; ++i)
    {
      if (isxdigit(str[i]) == 0)
        return 0;
    }
  
  return 1;
}


inline int
cond_pid(const char *str, unsigned int num)
{
  unsigned int i;

  if (strlen(str) != num)
    return 0;

  for (i = 0; i < num; ++i)
    if (isdigit(str[i]) == 0)
      return 0;
  
  return 1;
}

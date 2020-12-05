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
  "gnr",
  "hzl",
  "oth"
};

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
  { PASSPORT_HGT, "hgt", COND_HEIGHT,    heights          },
  { PASSPORT_HCL, "hcl", COND_COLOR,     NULL             },
  { PASSPORT_ECL, "ecl", COND_STR_RANGE, str_range        },
  { PASSPORT_PID, "pid", COND_PID,       NULL             },
  { PASSPORT_CID, "cid", COND_NONE,      NULL             },
};

int cond_range(const char *str, const int *range);
int cond_str_range(const char *str, const char **range);
int cond_height(const char *str, const int **range);
int cond_color(const char *str);
int cond_pid(const char *str);


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

  /* fseek(file, 0, SEEK_SET); */
  
  /* num = num_passport_correct_v2(file); */
  /* printf("Num v2: %d\n", num); */
  
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
          fprintf(stderr, "--------------------------- Passport : %X -- ", passport);
          if ((passport & PASSPORT_OK) == PASSPORT_OK)
            num++;

          fprintf(stderr, "Num : %d ---------------------------\n\n", num);
          passport = 0x0;
          cpy = NULL;
          continue;
        }

      cpy = strdup(str);
      for (tmp1 = cpy, token = NULL; ; tmp1 = NULL)
        {
          fprintf(stderr, "  -- passport : %X\n", passport);
          token = strtok_r(tmp1, " ", &saveptr1);
          if (token == NULL)
            break;

          fprintf(stderr, "Token : %s\n", token);
          
          for (tmp2 = token; ; tmp2 = NULL)
            {
              subtoken = strtok_r(tmp2, ":", &saveptr2);
              if (subtoken == NULL)
                break;

              fprintf(stderr, "\t subtoken : %s\n", subtoken);
              
              for (i = 0; i < SIZE(pf); ++i)
                if (strncmp(subtoken, pf[i].code, 3) == 0)
                  {
                    cond = 0;

                    switch (pf[i].condition)
                      {
                      case COND_RANGE:
                        cond = cond_range(subtoken, (int *) pf[i].cond_val);
                        break;

                      case COND_HEIGHT:
                        cond = cond_height(subtoken, (const int **) pf[i].cond_val);
                        break;

                      case COND_COLOR:
                        cond = cond_color(subtoken);
                        break;

                      case COND_STR_RANGE:
                        cond = cond_str_range(subtoken, (const char **) pf[i].cond_val);
                        break;

                      case COND_PID:
                        cond = cond_pid(subtoken);
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


int
cond_range(const char *str, const int *range)
{
  return 1; /* for compilation */
}

  
int cond_str_range(const char *str, const char **range)
{
  return 1; /* for compilation */
}


int
cond_height(const char *str, const int **heights)
{
  return 1; /* for compilation */
}


int
cond_color(const char *str)
{
  return 1; /* for compilation */
}


int
cond_pid(const char *str)
{
  return 1; /* for compilation */
}

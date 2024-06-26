#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "clean.h"

#define FAILED_READ 101
#define FAILED_OPEN 102
#define FAILED_MALLOC 103
#define INVALID_LINE 201
#define INVALID_SYMBOL 202

void error_out(int code, t_grid *grid)
{
  free_grid(grid);
  printf("%d", code);
  exit(1);
}
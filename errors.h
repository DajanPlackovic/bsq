#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"

#define FAILED_READ 101
#define FAILED_OPEN 102
#define FAILED_MALLOC 103
#define INVALID_LINE 201
#define INVALID_SYMBOL 202

void error_out(int code, t_grid *grid)
{
  int i;

  if (grid->fd != -1)
    close(grid->fd);
  if (grid->buffer)
    free(grid->buffer);
  if (grid && grid->numgrid)
  {
    i = 0;
    while (grid->numgrid[i])
    {
      free(grid->numgrid[i]);
      i++;
    }
  }
  printf("%d", code);
  exit(1);
}
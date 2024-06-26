#include <unistd.h>
#include <stdlib.h>
#include "grid.h"

void free_grid(t_grid *grid)
{
  int i;

  if (grid->fd != -1)
    close(grid->fd);
  if (grid->buffer)
    free(grid->buffer);
  if (grid->numgrid)
  {
    i = 0;
    while (i < grid->h && grid->numgrid[i])
    {
      free(grid->numgrid[i]);
      i++;
    }
  }
  free(grid->numgrid);
}
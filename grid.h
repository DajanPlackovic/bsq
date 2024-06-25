#ifndef GRID_H

#define GRID_H
typedef struct s_grid
{
  int h;
  int w;
  int **numgrid;
  int max;
  int x;
  int y;
  char empty;
  char obstacle;
  char fill;
  int fd;
  char *buffer;
} t_grid;

#endif
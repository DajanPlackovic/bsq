#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "grid.h"
#include "errors.h"

// read in first line
int file_open(t_grid *grid, char *path)
{
  grid->fd = open(path, O_RDONLY);
  if (grid->fd == -1)
    error_out(FAILED_OPEN, grid);
}

int check_nls(char *buffer)
{
  int i;
  int nls;

  i = 0;
  nls = 0;
  while (i < 500)
  {
    if (buffer[i] == '\n')
      nls++;
    if (nls == 2)
      return (nls);
    i++;
  }
  return (nls);
}

int get_config_length(t_grid *grid)
{
  int bytes_read;
  char buffer[2000];
  int total_read;
  int nls;

  bytes_read = 1;
  total_read = 0;
  nls = 0;
  while (bytes_read > 0 && nls < 2)
  {
    bytes_read = read(grid->fd, buffer, 2000);
    total_read += bytes_read;
    nls += check_nls(buffer);
  }
  if (bytes_read < 0)
    error_out(FAILED_READ, grid);
  return (total_read);
}

char *get_config(t_grid *grid, int clen)
{
  int status;
  char *config;

  config = (char *)malloc(clen + 1);
  if (!config)
    error_out(FAILED_MALLOC, grid);
  status = read(grid->fd, config, clen);
  if (status < 0)
  {
    free(config);
    error_out(FAILED_READ, grid);
  }
  config[clen + 1] = '\0';
  return (config);
}

int config_grid(t_grid *grid, char *config)
{
  int i;
  int j;

  grid->h = atoi(config);
  i = 0;
  while (config[i] >= '0' && config[i] <= '9')
    i++;
  grid->empty = config[i++];
  grid->obstacle = config[i++];
  grid->fill = config[i++];
  i++;
  j = 0;
  while (config[i + j] != '\n')
    j++;
  grid->w = j + 1;
  grid->max = 0;
  grid->x = 0;
  grid->y = 0;
  grid->numgrid = (int **)malloc(sizeof(int *) * grid->h);
  if (!grid->numgrid)
    error_out(FAILED_MALLOC, grid);
  j = 0;
  while (j < grid->h)
  {
    grid->numgrid[j] = 0;
    j++;
  }
  return (i);
}

int parse_config(t_grid *grid, char *path)
{
  int clen;
  char *config;

  file_open(grid, path);
  clen = get_config_length(grid);
  close(grid->fd);
  file_open(grid, path);
  config = get_config(grid, clen);
  clen = config_grid(grid, config);
  free(config);
  close(grid->fd);
  return (clen);
}
// read in line by line
void skip_config(t_grid *grid, int clen)
{
  int total_read;
  int bytes_read;
  char buffer[500];

  while (clen > 500)
  {
    bytes_read = read(grid->fd, buffer, 500);
    if (bytes_read < 0)
      error_out(FAILED_READ, grid);
    clen -= bytes_read;
  }
  if (clen != 0)
    bytes_read = read(grid->fd, buffer, clen);
  if (bytes_read < 0)
    error_out(FAILED_READ, grid);
}
// validate
int valid_line(t_grid *grid)
{
  if (grid->buffer[grid->w - 1] == '\n')
    return (1);
  return (0);
}
// parse
// populate solution grid
void solve_line(t_grid *grid, int line_num)
{
  int i;
  int min;

  i = 0;
  grid->numgrid[line_num] = (int *)malloc(sizeof(int) * grid->w);
  if (!grid->numgrid[line_num])
    error_out(FAILED_MALLOC, grid);
  while (i < grid->w - 1)
  {
    if (grid->buffer[i] == grid->empty)
    {
      if (line_num == 0)
        grid->numgrid[line_num][i] = 1;
      else
      {
        min = grid->numgrid[line_num - 1][i];
        if (i && grid->numgrid[line_num][i - 1] < min)
          min = grid->numgrid[line_num][i - 1];
        if (i && grid->numgrid[line_num - 1][i - 1] < min)
          min = grid->numgrid[line_num - 1][i - 1];
        grid->numgrid[line_num][i] = min + 1;
      }
      if (grid->numgrid[line_num][i] > grid->max)
      {
        grid->max = grid->numgrid[line_num][i];
        grid->x = i;
        grid->y = line_num;
      }
    }
    else if (grid->buffer[i] == grid->obstacle)
      grid->numgrid[line_num][i] = 0;
    else
      error_out(INVALID_SYMBOL, grid);
    i++;
  }
}
// empty = 1
// obstacle = 0
void solve(t_grid *grid, char *path)
{
  int clen;
  int bytes_read;
  int line_num;

  clen = parse_config(grid, path);
  file_open(grid, path);
  skip_config(grid, clen);
  grid->buffer = (char *)malloc(grid->w);
  if (!grid->buffer)
    error_out(FAILED_MALLOC, grid);
  bytes_read = 1;
  line_num = 0;
  while (bytes_read > 0)
  {
    bytes_read = read(grid->fd, grid->buffer, grid->w);
    if (!valid_line(grid))
      error_out(INVALID_LINE, grid);
    solve_line(grid, line_num);
    line_num++;
  }
  if (bytes_read < 0)
    error_out(FAILED_READ, grid);
  close(grid->fd);
}

void print_grid(t_grid grid)
{
  int i;
  int j;
  int top;
  int left;

  top = grid.y - grid.max;
  left = grid.x - grid.max;
  i = 0;
  while (i < grid.h)
  {
    j = 0;
    while (j < grid.w - 1)
    {
      if (i > top && i < grid.y && j > left && j < grid.x)
        printf("%c", grid.fill);
      else if (grid.numgrid[i][j] == 0)
        printf("%c", grid.obstacle);
      else
        printf("%c", grid.empty);
      j++;
    }
    printf("\n");
    i++;
  }
}

int main(void)
{
  t_grid grid;

  solve(&grid, "map");
  print_grid(grid);
  return (0);
}

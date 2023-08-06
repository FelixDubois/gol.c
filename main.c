#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 40

typedef struct {
  bool map[MAP_WIDTH * MAP_HEIGHT];
  int step;
} life;

life gol;

int count_neighbors(int x, int y) {
  int count = 0;

  count += gol.map[y * MAP_WIDTH + (x - 1 + MAP_WIDTH) % MAP_WIDTH];
  count += gol.map[y * MAP_WIDTH + (x + 1) % MAP_WIDTH];
  count += gol.map[(y + 1) % MAP_HEIGHT * MAP_WIDTH + x];
  count += gol.map[(y - 1 + MAP_HEIGHT) % MAP_HEIGHT * MAP_WIDTH + x];

  count += gol.map[(y - 1 + MAP_HEIGHT) % MAP_HEIGHT * MAP_WIDTH +
                   (x - 1 + MAP_WIDTH) % MAP_WIDTH];
  count += gol.map[(y + 1) % MAP_HEIGHT * MAP_WIDTH + (x + 1) % MAP_WIDTH];
  count += gol.map[(y + 1) % MAP_HEIGHT * MAP_WIDTH +
                   (x - 1 + MAP_WIDTH) % MAP_WIDTH];
  count += gol.map[(y - 1 + MAP_HEIGHT) % MAP_HEIGHT * MAP_WIDTH +
                   (x + 1) % MAP_WIDTH];

  return count;
}

void step() {
  life ngol;
  for (size_t y = 0; y < MAP_HEIGHT; ++y) {
    for (size_t x = 0; x < MAP_WIDTH; ++x) {
      int c = count_neighbors(x, y);

      int s = gol.map[y * MAP_WIDTH + x];
      ngol.map[y * MAP_WIDTH + x] = (c == 3 || (s && c == 2));
    }
  }

  ngol.step = gol.step + 1;
  gol = ngol;
}

void print_map() {
  printf("┏");
  for (size_t i = 1; i <= MAP_WIDTH; ++i) {
    printf("━");
  }

  printf("┓\n");

  for (size_t y = 0; y < MAP_HEIGHT; ++y) {
    for (size_t x = 0; x < MAP_WIDTH; ++x) {
      if (x == 0)
        printf("┃");

      if (gol.map[y * MAP_WIDTH + x])
        printf("█");
      else
        printf(" ");

      if (x == MAP_WIDTH - 1)
        printf("┃");
    }
    printf("\n");
  }

  printf("┗");
  for (size_t x = 1; x <= MAP_WIDTH; ++x) {
    printf("━");
  }
  printf("┛\n");
}

void generate_random_map(float density) {

  for (size_t y = 0; y < MAP_HEIGHT; ++y) {
    for (size_t x = 0; x < MAP_WIDTH; ++x) {
      float p = (float)rand() / RAND_MAX;
      gol.map[y * MAP_WIDTH + x] = p < density;
    }
  }
}

int main() {
  srand(time(0));

  generate_random_map(0.1);

  while (1) {
    printf("Step : %i\n", gol.step);
    print_map();
    step();
    usleep(100 * 1000);
  }
}

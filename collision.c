#include "collision.h"
#include <math.h>
#include <stdio.h>
// first shot at grid-based collision

extern Particle particles[NUM_PARTICLES];
const float EPS = 0.0001f;

int grid[GRID_COLS * GRID_ROWS];
int next[NUM_PARTICLES];

void GridUpdate() {
  // first we gotta allocate particles into grid cells 1 by 1
  for (int i = 0; i < GRID_COLS * GRID_ROWS; ++i) {
    grid[i] = -1;
  }

  for (int i = 0; i < NUM_PARTICLES; ++i) {
    // fill the grid with the most recently checked particle
    int row = particles[i].y / CELL_SIZE;
    int col = particles[i].x / CELL_SIZE;
    int index = row * GRID_COLS + col;
    next[i] = grid[index]; // save the position of the particle that was here
                           // before in next[]
    grid[index] = i;
    // this is essentially a linked list implementation that doesn't require
    // dynamic memory allocation so we can start at the first particle which
    // will actually be in the grid array, then follow the chain till we hit a
    // -1, which is the tail of the linked list.
  }
}

void CollideParticles(Particle *a, Particle *b) {
  Vector2 c1 = {a->x, a->y};
  Vector2 c2 = {b->x, b->y};

  if (!CheckCollisionCircles(c1, a->r, c2, b->r))
    return;

  float dx = a->x - b->x;
  float dy = a->y - b->y;
  float dist = sqrtf(dx * dx + dy * dy);
  if (dist < EPS)
    return;

  float nx = dx / dist;
  float ny = dy / dist;

  float overlap = (a->r + b->r) - dist;
  a->x += nx * overlap * 0.5f;
  a->y += ny * overlap * 0.5f;
  b->x -= nx * overlap * 0.5f;
  b->y -= ny * overlap * 0.5f;

  float rvx = a->vx - b->vx;
  float rvy = a->vy - b->vy;
  float rel = rvx * nx + rvy * ny;
  if (rel > 0)
    return;

  float tx = -ny;
  float ty = nx;

  float v1t = a->vx * tx + a->vy * ty;
  float v2t = b->vx * tx + b->vy * ty;
  float v1n = a->vx * nx + a->vy * ny;
  float v2n = b->vx * nx + b->vy * ny;

  float tmp = v1n;
  v1n = v2n;
  v2n = tmp;

  a->vx = DAMPENING_FACTOR * (v1t * tx + v1n * nx);
  a->vy = DAMPENING_FACTOR * (v1t * ty + v1n * ny);
  b->vx = DAMPENING_FACTOR * (v2t * tx + v2n * nx);
  b->vy = DAMPENING_FACTOR * (v2t * ty + v2n * ny);
}

void CollideAllParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    int col = particles[i].x / CELL_SIZE;
    int row = particles[i].y / CELL_SIZE;

    if (col < 0 || col >= GRID_COLS || row < 0 || row >= GRID_ROWS)
      continue;

    for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
        int newCol = col + dx;
        int newRow = row + dy;
        if (newCol < 0 || newCol >= GRID_COLS || newRow < 0 ||
            newRow >= GRID_ROWS)
          continue;
        int cell = newRow * GRID_COLS + newCol;
        int neighbourIndex = grid[cell];
        while (neighbourIndex != -1) {
          CollideParticles(&particles[neighbourIndex], &particles[i]);
          neighbourIndex = next[neighbourIndex];
        }
      }
    }
  }
}

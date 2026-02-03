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
  float dx = b->x - a->x;
  float dy = b->y - a->y;
  float dist = sqrtf(dx * dx + dy * dy);
  float minDist = a->r + b->r;
  if (dist <= 0.0001f || dist >= minDist)
    return;
  float nx = dx / dist;
  float ny = dy / dist;
  float overlap = minDist - dist;
  float m1 = a->r * a->r;
  float m2 = b->r * b->r;
  float inv1 = 1.0f / m1;
  float inv2 = 1.0f / m2;
  float corr = overlap / (inv1 + inv2) * 0.1f;
  a->x -= nx * corr * inv1;
  a->y -= ny * corr * inv1;
  b->x += nx * corr * inv2;
  b->y += ny * corr * inv2;
  float rvx = b->vx - a->vx;
  float rvy = b->vy - a->vy;
  float rel = rvx * nx + rvy * ny;
  if (rel > 0.0f)
    return;
  float e = 0.8f;
  float j = -(1.0f + e) * rel / (inv1 + inv2);
  float jx = j * nx;
  float jy = j * ny;
  a->vx -= jx * inv1;
  a->vy -= jy * inv1;
  b->vx += jx * inv2;
  b->vy += jy * inv2;
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

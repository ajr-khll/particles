#pragma once

#define WIDTH 1920
#define HEIGHT 1080
#define GRID_COLS 128
#define GRID_ROWS 72
#define CELL_SIZE 15
#define NUM_PARTICLES 20000
#define DAMPENING_FACTOR 0.65
#define GRAVITY 0.1

typedef struct {
  float x, y, r, vx, vy;
} Particle;

void CollideAllParticles();

void GridUpdate();

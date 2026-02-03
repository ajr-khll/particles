#pragma once
#include "raylib.h"
#define WIDTH 1920
#define HEIGHT 1080
#define GRID_COLS 160
#define GRID_ROWS 90
#define CELL_SIZE 12
#define NUM_PARTICLES 10000
#define DAMPENING_FACTOR 0.99
#define GRAVITY 0.1

typedef struct {
  float x, y, r, vx, vy;
  Color color;
} Particle;

void CollideAllParticles();

void GridUpdate();

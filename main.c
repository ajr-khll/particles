#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define WIDTH 1920
#define HEIGHT 1080

#define NUM_PARTICLES 2000
#define DAMPENING_FACTOR 0.98
#define GRAVITY 0.1

typedef struct {
  float x, y, r, vx, vy;
} Particle;

Particle particles[NUM_PARTICLES];

void DrawParticle(Particle *particle) {
  DrawCircle(particle->x, particle->y, particle->r, BLACK);
}

void DrawParticles() {
  for (int i = 0; i <= NUM_PARTICLES; i++) {
    DrawParticle(&particles[i]);
  }
}

void UpdateParticle(Particle *particle) {
  particle->vy += GRAVITY;
  particle->x += particle->vx;
  particle->y += particle->vy;

  float x = particle->x;
  float y = particle->y;
  float r = particle->r;
  // wall collision
  if ((x + r > WIDTH)) {
    particle->vx = -particle->vx;
    particle->x = WIDTH - r;
  } else if ((x - r < 0)) {
    particle->vx = -particle->vx;
    particle->x = r;
  };
  if ((y + r > HEIGHT)) {
    particle->vy = -particle->vy;
    particle->y = HEIGHT - r;
  } else if ((y - r < 0)) {
    particle->vy = -particle->vy;
    particle->y = r;
  };
}

void UpdateParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    UpdateParticle(particles + i);
  }
}

void CollideAllParticles() {
  const float EPS = 0.0001f;

  for (int i = 0; i < NUM_PARTICLES; i++) {
    for (int j = i + 1; j < NUM_PARTICLES; j++) {

      Particle *a = &particles[i];
      Particle *b = &particles[j];

      Vector2 c1 = {a->x, a->y};
      Vector2 c2 = {b->x, b->y};

      if (!CheckCollisionCircles(c1, a->r, c2, b->r))
        continue;

      float dx = a->x - b->x;
      float dy = a->y - b->y;
      float dist = sqrtf(dx * dx + dy * dy);
      if (dist < EPS)
        continue;

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
        continue;

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
  }
}
void InitParticles() {
  SetRandomSeed(time(NULL));
  float radius;
  for (int i = 0; i < NUM_PARTICLES; i++) {
    // radius = GetRandomValue(0, 15);
    radius = 12;
    particles[i].r = radius;
    particles[i].x = GetRandomValue(radius, WIDTH - radius);
    particles[i].y = GetRandomValue(radius, HEIGHT - radius);
    particles[i].vx = GetRandomValue(-5, 5);
    particles[i].vy = GetRandomValue(-5, 5);
  }
}

int main(void) {

  InitWindow(WIDTH, HEIGHT, "Particle Sim");
  InitParticles();

  SetTargetFPS(60);
  Particle particle = {300, 300, 50, 40, 40};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateParticles();
    CollideAllParticles();
    DrawParticles();
    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

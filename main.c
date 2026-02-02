#include "collision.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Particle particles[NUM_PARTICLES];

void DrawParticle(Particle *particle) {
  DrawCircle(particle->x, particle->y, particle->r, BLACK);
}

void DrawParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
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

void InitParticles() {
  SetRandomSeed(time(NULL));
  float radius;
  for (int i = 0; i < NUM_PARTICLES; i++) {
    // radius = GetRandomValue(0, 15);
    radius = 4;
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
    GridUpdate();
    CollideAllParticles();
    DrawParticles();
    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

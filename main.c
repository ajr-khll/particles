#include "raylib.h"

#define WIDTH 800
#define HEIGHT 600

#define NUM_PARTICLES 15

typedef struct {
  float x, y, r, vx, vy;
} Particle;

Particle particles[NUM_PARTICLES];

void DrawParticle(Particle *particle) {
  DrawCircle(particle->x, particle->y, particle->r, BLACK);
}

void UpdateParticle(Particle *particle) {
  particle->x += particle->vx;
  particle->y += particle->vy;

  float x = particle->x;
  float y = particle->y;
  float r = particle->r;
  // wall collision
  if ((x + r >= WIDTH) || (x - r <= 0)) {
    particle->vx = -particle->vx;
  }
  if ((y + r >= HEIGHT) || (y - r <= 0)) {
    particle->vy = -particle->vy;
  }
}

int main(void) {

  InitWindow(WIDTH, HEIGHT, "Particle Sim");
  SetTargetFPS(60);
  Particle particle = {300, 300, 50, 40, 40};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    UpdateParticle(&particle);
    DrawParticle(&particle);
    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

#include "collision.h"
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

Particle particles[NUM_PARTICLES];
int particlesSpawned = 0;

void DrawParticle(Particle *particle) {
  DrawCircle(particle->x, particle->y, particle->r, particle->color);
}

void DrawParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    DrawParticle(&particles[i]);
  }
}

void UpdateParticle(Particle *particle) {
  float dt = GetFrameTime();
  particle->vy += GRAVITY * dt;
  particle->x += particle->vx * dt;
  particle->y += particle->vy * dt;
  float x = particle->x;
  float y = particle->y;
  float r = particle->r;
  const float REST = 0.75f;
  if (x + r > WIDTH) {
    particle->x = WIDTH - r;
    particle->vx = -particle->vx * REST;
  } else if (x - r < 0) {
    particle->x = r;
    particle->vx = -particle->vx * REST;
  }
  if (y + r > HEIGHT) {
    particle->y = HEIGHT - r;
    particle->vy = -particle->vy * REST;
  } else if (y - r < 0) {
    particle->y = r;
    particle->vy = -particle->vy * REST;
  }
}

void UpdateParticles() {
  for (int i = 0; i < NUM_PARTICLES; i++) {
    UpdateParticle(particles + i);
  }
}

void InitParticles() {
  SetRandomSeed(time(NULL));
  float sx = WIDTH - 40.0f;
  float sy = 20.0f;
  for (int i = 0; i < NUM_PARTICLES; i++) {
    particles[i].r = 4.0f;
    particles[i].x = sx + GetRandomValue(-6, 6);
    particles[i].y = sy + GetRandomValue(-6, 6);
    float speed = GetRandomValue(400, 700);
    float angle = (135.0f + GetRandomValue(-20, 20)) * DEG2RAD;
    particles[i].vx = cosf(angle) * speed;
    particles[i].vy = sinf(angle) * speed;
    particles[i].color =
        (Color){GetRandomValue(50, 255), GetRandomValue(50, 255),
                GetRandomValue(50, 255), 255};
  }
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Particle Sim");
  InitParticles();

  SetTargetFPS(60);
  Particle particle = {300, 300, 50, 40, 40};

  while (!WindowShouldClose()) {
    if (!particlesSpawned && IsKeyPressed(KEY_SPACE)) {
      InitParticles();
      particlesSpawned = 1;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    if (particlesSpawned) {
      UpdateParticles();
      GridUpdate();
      CollideAllParticles();
      DrawParticles();
    }

    DrawFPS(5, 5);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}

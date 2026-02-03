# Particles

This is a C + raylib particle simulation: 10,000 circles with gravity, wall
bounces, and grid-accelerated collisions.

PRESS SPACE TO SPAWN THE PARTICLES

## Highlights
- 10,000 particles with real-time collisions via a uniform grid.
- Simple physics: gravity, wall bounces, and elastic-ish swaps.
- Compact C11 codebase that is easy to tune.

## Build + Run
Requires raylib and CMake.

```bash
cmake -S . -B build
cmake --build build
./build/MyRaylibGame
```

## Controls
- Close the window or press Esc to quit.

## Tuning Knobs
All the constants that matter live in `collision.h`:
- `NUM_PARTICLES` (default 10000)
- `GRAVITY`
- `DAMPENING_FACTOR`
- `WIDTH` / `HEIGHT`
- Grid settings: `GRID_COLS`, `GRID_ROWS`, `CELL_SIZE`

## Notes
The collision pass uses a grid + linked-list approach to avoid O(n^2) checks
every frame. Each particle only checks its own cell and neighbors.

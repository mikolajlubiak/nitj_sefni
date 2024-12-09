
// local
#include "entity.hpp"
#include "world.hpp"

// std
#include <iostream>

int main() {
  nitjsefni::World world;
  world.PopulateWorld(FastNoiseLite::NoiseType_OpenSimplex2);
  auto world_grid = world.GetWorld();

  for (const auto &row : world_grid) {
    for (const auto cell : row) {
      std::cout << cell << " ";
    }
    std::cout << std::endl;
  }

  return EXIT_SUCCESS;
}

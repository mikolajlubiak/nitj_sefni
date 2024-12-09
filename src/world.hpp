#pragma once

// local
#include "utility.hpp"

// libs
// FastNoiseLite
#include "FastNoiseLite.h"

// std
#include <array>
#include <cstdint>

namespace nitjsefni {

constexpr std::size_t kWorldSize = 64;

enum Cell : std::uint32_t {
  kGrass,
  kWater,
  kTree,
  kCellLength,
};

class World {
public:
  void PopulateWorld(FastNoiseLite::NoiseType noise_type) {
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(noise_type);

    // Gather noise data
    for (std::size_t y = 0; y < kWorldSize; y++) {
      for (std::size_t x = 0; x < kWorldSize; x++) {
        world_grid[y][x] = MapValue(
            noise.GetNoise(static_cast<float>(x), static_cast<float>(y)), -1.0f,
            1.0f, static_cast<Cell>(0), kCellLength);
      }
    }
  }

  const std::array<std::array<Cell, kWorldSize>, kWorldSize> &GetWorld() const {
    return world_grid;
  }

private:
  std::array<std::array<Cell, kWorldSize>, kWorldSize> world_grid;
};

} // namespace nitjsefni

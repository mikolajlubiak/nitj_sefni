#pragma once

// local
#include "utility.hpp"

// libs
// FastNoiseLite
#include "FastNoiseLite.h"

// std
#include <array>
#include <cmath>
#include <cstdint>

namespace nitjsefni {

constexpr std::size_t kWorldSize = 256;

enum Cell : std::uint8_t {
  kWater,
  kGrass,
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
        float sample = MapValue(
            noise.GetNoise(static_cast<float>(x), static_cast<float>(y)), -1.0f,
            1.0f, 0.0f, static_cast<float>(kCellLength - 1));

        world_grid[y][x] = static_cast<Cell>(std::round(sample));
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

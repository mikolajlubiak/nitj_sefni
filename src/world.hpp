#pragma once

// local
#include "utility.hpp"

// libs
// FastNoiseLite
#include "FastNoiseLite.h"

// std
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <random>
#include <string>

namespace nitjsefni {

constexpr std::size_t kWorldSize = 256;

enum CellType : std::uint8_t {
  kWater,
  kGrass,
  kTree,
  kCellLength,
};

struct Cell {
  CellType type;
  std::string symbol;
  std::array<std::uint8_t, 3> color;
};

class World {
public:
  void PopulateWorld(FastNoiseLite::NoiseType noise_type) {
    // Create and configure FastNoise object
    FastNoiseLite noise;
    noise.SetNoiseType(noise_type);

    constexpr std::array possible_grass_symbols = {",", ".", ""};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> grass_dist(
        0, possible_grass_symbols.size() - 1);

    // Gather noise data
    for (std::size_t y = 0; y < kWorldSize; y++) {
      for (std::size_t x = 0; x < kWorldSize; x++) {
        const float mapped_sample = MapValue(
            noise.GetNoise(static_cast<float>(x), static_cast<float>(y)), -1.0f,
            1.0f, 0.0f, static_cast<float>(kCellLength - 1));

        const CellType cell_type =
            static_cast<CellType>(std::round(mapped_sample));

        std::string symbol = "#";
        std::array<std::uint8_t, 3> color{};

        switch (cell_type) {
        case CellType::kGrass:
          color = {0, 255, 0};
          symbol = possible_grass_symbols[grass_dist(gen)];
          break;
        case CellType::kWater:
          color = {0, 0, 255};
          symbol = "~";
          break;
        case CellType::kTree:
          color = {165, 42, 42};
          symbol = "▲";
          break;
        }

        const float color_intensitivity =
            1.0f - std::abs(std::round(mapped_sample) - (mapped_sample));

        std::transform(color.begin(), color.end(), color.begin(),
                       [color_intensitivity](const std::uint8_t x) {
                         return x * color_intensitivity;
                       });

        world_grid[y][x] = {cell_type, symbol, color};
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

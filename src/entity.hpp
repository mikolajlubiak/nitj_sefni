#pragma once

// local
#include "utility.hpp"

// std
#include <array>
#include <cstdint>

namespace nitjsefni {

class Entity {
public:
  Entity() = default;
  Entity(std::array<float, 2> position) : position(position) {}
  Entity(std::array<float, 2> position, float health, float strength,
         float defense)
      : position(position), health(health), strength(strength),
        defense(defense) {}

  void SubtractHealth(const float damage) { this->health -= damage; }

  void SubtractStrength(const float weakening) { this->strength -= weakening; }

  void SubtractDefense(const float weakening) { this->defense -= weakening; }

  void SetPosition(std::array<float, 2> position) { this->position = position; }

  const std::array<float, 2> &GetPosition() const { return position; }

private:
  float health;
  float strength;
  float defense;

  std::array<float, 2> position;
};

} // namespace nitjsefni

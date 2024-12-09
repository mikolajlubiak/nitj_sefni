#pragma once

// local
#include "utility.hpp"

// std
#include <cstdint>

namespace nitjsefni {

class Entity {
public:
  void SubtractHealth(const std::uint32_t damage) {
    health = UnsignedSubstraction(health, damage);
  }

  void SubtractStrength(const std::uint32_t weakening) {
    strength = UnsignedSubstraction(strength, weakening);
  }

  void SubtractDefense(const std::uint32_t weakening) {
    defense = UnsignedSubstraction(defense, weakening);
  }

private:
  std::uint32_t health;
  std::uint32_t strength;
  std::uint32_t defense;
};

} // namespace nitjsefni

#pragma once

template <typename unsigned_integer, typename any_integer>
unsigned_integer UnsignedSubstraction(const unsigned_integer a,
                                      const any_integer b) {
  if (a <= b) [[unlikely]] {
    return 0;
  }

  return a - b;
}

template <typename T>
T MapValue(T x, T old_min, T old_max, T new_min, T new_max) {
  return new_min + (x - old_min) * (new_max - new_min) / (old_max - old_min);
}

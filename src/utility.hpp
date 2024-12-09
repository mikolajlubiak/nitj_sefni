#pragma once

template <typename unsigned_integer, typename any_integer>
unsigned_integer UnsignedSubstraction(const unsigned_integer a,
                                      const any_integer b) {
  if (a <= b) [[unlikely]] {
    return 0;
  }

  return a - b;
}

template <typename old_type, typename new_type>
new_type MapValue(old_type x, old_type old_min, old_type old_max,
                  new_type new_min, new_type new_max) {
  return static_cast<new_type>(new_min + (x - old_min) * (new_max - new_min) /
                                             (old_max - old_min));
}

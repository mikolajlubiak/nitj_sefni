template <typename T1, typename T2>
T1 UnsignedSubstraction(const T1 a, const T2 b) {
  if (a <= b) [[unlikely]] {
    return 0;
  }

  return a - b;
}

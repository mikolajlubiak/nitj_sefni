// local
#include "terminal_renderer.hpp"

// std
#include <iostream>

int main() {

  nitjsefni::TerminalRenderer terminal_renderer{};

  terminal_renderer.loop();

  return EXIT_SUCCESS;
}

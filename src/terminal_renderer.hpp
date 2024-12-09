#pragma once

// local
#include "world.hpp"

// libs
// FTXUI
#include <ftxui/component/component.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

namespace nitjsefni {

class TerminalRenderer {
public:
  TerminalRenderer() {
    world->PopulateWorld(FastNoiseLite::NoiseType_OpenSimplex2);
  }

  void loop() {
    auto world_component = WorldComponent();

    screen.Loop(world_component);
  }

  ftxui::Component WorldComponent() const {
    auto world_grid = world->GetWorld();

    auto world_renderer = ftxui::Renderer([&] {
      // Scale to fit canvas 2x4 braille dot
      std::size_t width = screen.dimx() * 2;
      std::size_t height = screen.dimy() * 4;

      // Initialize dynamic canvas component
      auto c = ftxui::Canvas(width, height);

      for (std::size_t y = 0; y < screen.dimy(); y++) {
        for (std::size_t x = 0; x < screen.dimx(); x++) {
          ftxui::Color color = ftxui::Color(0, 0, 0);
          std::string character = "#";

          if (y < world_grid.size()) {
            if (x < world_grid[y].size()) {
              Cell cell = world_grid[y][x];

              switch (cell) {
              case Cell::kGrass:
                color = ftxui::Color(0, 255, 0);
                character = ".";
                break;
              case Cell::kWater:
                color = ftxui::Color(0, 0, 255);
                character = "~";
                break;
              case Cell::kTree:
                color = ftxui::Color(165, 42, 42);
                character = "▲";
                break;
              default:
                color = ftxui::Color(0, 0, 0);
                character = "#";
                break;
              }
            }
          }

          // x and y are assumed to be multiples of 2 and 4 respectively
          c.DrawText(x * 2, y * 4, character, color);
        }
      }
      return ftxui::canvas(std::move(c));
    });

    return world_renderer;
  }

private:
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

  std::shared_ptr<World> world = std::make_shared<World>();
};

} // namespace nitjsefni

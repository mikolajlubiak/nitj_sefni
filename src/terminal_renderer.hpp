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

    std::int32_t offset_x = 0;
    std::int32_t offset_y = 0;

    std::int32_t player_x;
    std::int32_t player_y;

    auto world_renderer = ftxui::Renderer([&] {
      // Scale to fit canvas 2x4 braille dot
      std::size_t width = screen.dimx() * 2;
      std::size_t height = screen.dimy() * 4;

      player_x = screen.dimx() / 2 + offset_x;
      player_y = screen.dimy() / 2 + offset_y;

      // Initialize dynamic canvas component
      auto c = ftxui::Canvas(width, height);

      for (std::size_t y = 0; y < screen.dimy(); y++) {
        for (std::size_t x = 0; x < screen.dimx(); x++) {
          ftxui::Color color = ftxui::Color(0, 0, 0);
          std::string symbol = "#";

          const std::int32_t world_x = x + offset_x;
          const std::int32_t world_y = y + offset_y;

          if (y == screen.dimy() / 2 && x == screen.dimx() / 2) [[unlikely]] {
            color = ftxui::Color(255, 255, 255);
          } else if (world_y < world_grid.size()) {
            if (world_x < world_grid[world_y].size()) {
              Cell cell = world_grid[world_y][world_x];

              color = ftxui::Color(cell.color[0], cell.color[1], cell.color[2]);
              symbol = cell.symbol;
            }
          }

          // x and y are assumed to be multiples of 2 and 4 respectively
          c.DrawText(x * 2, y * 4, symbol, color);
        }
      }
      return ftxui::canvas(std::move(c));
    });

    world_renderer |= ftxui::CatchEvent([&](ftxui::Event e) {
      if (e == ftxui::Event::Character('w')) {
        if (player_y > 0 &&
            world_grid[player_y - 1][player_x].type == CellType::kGrass) {
          offset_y--;
          player_y--;
        }
        return true;
      }
      if (e == ftxui::Event::Character('s')) {
        if (player_y + 1 < world_grid.size() &&
            world_grid[player_y + 1][player_x].type == CellType::kGrass) {
          offset_y++;
          player_y++;
        }
        return true;
      }
      if (e == ftxui::Event::Character('a')) {
        if (player_x > 0 &&
            world_grid[player_y][player_x - 1].type == CellType::kGrass) {
          offset_x--;
          player_x--;
        }
        return true;
      }
      if (e == ftxui::Event::Character('d')) {
        if (player_x + 1 < world_grid[player_y].size() &&
            world_grid[player_y][player_x + 1].type == CellType::kGrass) {
          offset_x++;
          player_x++;
        }
        return true;
      }

      return false;
    });

    return world_renderer;
  }

private:
  ftxui::ScreenInteractive screen = ftxui::ScreenInteractive::Fullscreen();

  std::shared_ptr<World> world = std::make_shared<World>();
};

} // namespace nitjsefni

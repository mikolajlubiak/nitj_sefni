#pragma once

// local
#include "entity.hpp"
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

    Entity player({100, 25}, 100, 5, 3);
    Entity camera({0, 0}, 0, 0, 0);

    std::array<float, 2> player_position = player.GetPosition();
    std::array<float, 2> camera_position = camera.GetPosition();

    auto world_renderer = ftxui::Renderer([&] {
      // Scale to fit canvas 2x4 braille dot
      std::size_t width = screen.dimx() * 2;
      std::size_t height = screen.dimy() * 4;

      // Initialize dynamic canvas component
      auto c = ftxui::Canvas(width, height);

      player_position = player.GetPosition();
      camera_position = camera.GetPosition();

      for (std::size_t y = 0; y < screen.dimy(); y++) {
        for (std::size_t x = 0; x < screen.dimx(); x++) {
          ftxui::Color color = ftxui::Color(0, 0, 0);
          std::string symbol = "#";

          const std::int32_t world_x = x + camera_position[0];
          const std::int32_t world_y = y + camera_position[1];

          if (world_x == player_position[0] && world_y == player_position[1])
              [[unlikely]] {
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
        if (player_position[1] > 0 &&
            world_grid[player_position[1] - 1][player_position[0]].type ==
                CellType::kGrass) {
          player.SetPosition({player_position[0], player_position[1] - 1});
          camera.SetPosition({camera_position[0], camera_position[1] - 1.1f});
        }
        return true;
      }
      if (e == ftxui::Event::Character('s')) {
        if (player_position[1] + 1 < world_grid.size() &&
            world_grid[player_position[1] + 1][player_position[0]].type ==
                CellType::kGrass) {
          player.SetPosition({player_position[0], player_position[1] + 1});
          camera.SetPosition({camera_position[0], camera_position[1] + 1.1f});
        }
        return true;
      }
      if (e == ftxui::Event::Character('a')) {
        if (player_position[0] > 0 &&
            world_grid[player_position[1]][player_position[0] - 1].type ==
                CellType::kGrass) {
          player.SetPosition({player_position[0] - 1, player_position[1]});
          camera.SetPosition({camera_position[0] - 1.1f, camera_position[1]});
        }
        return true;
      }
      if (e == ftxui::Event::Character('d')) {
        if (player_position[0] + 1 < world_grid[player_position[1]].size() &&
            world_grid[player_position[1]][player_position[0] + 1].type ==
                CellType::kGrass) {
          player.SetPosition({player_position[0] + 1, player_position[1]});
          camera.SetPosition({camera_position[0] + 1.1f, camera_position[1]});
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

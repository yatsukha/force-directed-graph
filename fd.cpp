#include "fd.hpp"

#include <iostream>
#include <string>

int main(int argc, char const* const* argv) {

  ::fd::graph g;
  g.p = {
    {0.0f, 0.0f},
    {-0.5f, 0.0f},
    {0.5f, 0.0f},
    {0.0f, 0.5f},
    {0.0f, -0.5f}
  };
  g.c = {
    {{1, 2, 3, 4}},
    {{0}},
    {{0}},
    {{0}},
    {{0}}
  };
  g.d = {2.0f, 2.0f};

  auto t = 0.1f;

  for (auto i = 0; i < ::std::stoi(argv[1]); ++i) {
    for (auto [x, y] : g.p)
      ::std::cout << "(" << x << ", " << y << ")" << "\n";
    g = ::fd::advance(g, t);
    t *= 0.95;
    ::std::cout << "\n";
  }

  for (auto [x, y] : g.p)
    ::std::cout << "(" << x << ", " << y << ")" << "\n";

}
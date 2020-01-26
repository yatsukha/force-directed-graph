#include "fd.hpp"

#include <iostream>
#include <string>

int main(int argc, char const* const* argv) {

  ::graph g{
    {
      {0.0f, 0.0f},
      {-0.5f, 0.0f},
      {0.5f, 0.0f},
      {0.0f, 0.5f},
      {0.0f, -0.5f}
    },
    {
      {0, {1, 2, 3, 4}},
      {1, {0}},
      {2, {0}},
      {3, {0}},
      {4, {0}}
    },
    {2.0f, 2.0f}
  };

  auto t = 0.1f;

  for (auto i = 0; i < ::std::stoi(argv[1]); ++i) {
    for (auto [x, y] : ::std::get<0>(g))
      ::std::cout << "(" << x << ", " << y << ")" << "\n";
    g = ::advance(g, t);
    t *= 0.95;
    ::std::cout << "\n";
  }

  for (auto [x, y] : ::std::get<0>(g))
    ::std::cout << "(" << x << ", " << y << ")" << "\n";

}
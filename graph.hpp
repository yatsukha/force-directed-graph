#pragma once

#include <map>
#include <vector>
#include <random>
#include <string>
#include <iostream>
#include <algorithm>

#include <emscripten/bind.h>

#include "fd.hpp"

// used as a closed namespace
class graph final {
 private:
  static ::fd::graph g;

 public:
  static void add_point(::fd::unit const x, ::fd::unit const y, 
                        ::std::vector<::fd::vertex> const neighbours) {
    auto& [pos, connections, _] = g;
    auto const id = pos.size();
    for (auto const nb : neighbours)
      connections[nb].push_back(id);
    connections.emplace_back(::std::move(neighbours));
    pos.push_back({x, y});
  }

  static void connect(::fd::vertex const u, ::fd::vertex const v) {
    if (::std::find(g.c[u].begin(), g.c[u].end(), v) == g.c[u].end())
      g.c[u].push_back(v);
    if (::std::find(g.c[v].begin(), g.c[v].end(), u) == g.c[v].end())
      g.c[v].push_back(u);
  }

  static void disconnect(::fd::vertex const u, ::fd::vertex const v) {
    if (auto iter = ::std::find(g.c[u].begin(), g.c[u].end(), v); 
        iter != g.c[u].end())
      g.c[u].erase(iter);
    if (auto iter = ::std::find(g.c[v].begin(), g.c[v].end(), u);
        iter != g.c[v].end())
      g.c[v].erase(iter);
  }

  static ::fd::graph const* get() noexcept {
    return &g;
  }

  static void advance(::fd::unit const t) {
    g = ::fd::advance(g, t); // 0.001
  }

};

using namespace ::emscripten;

EMSCRIPTEN_BINDINGS(graph_binding) {
  value_object<::fd::point>("point")
    .field("x", &::fd::point::first)
    .field("y", &::fd::point::second);

  register_vector<::fd::point>("PointVector");
  register_vector<::fd::vertex>("VertexVector");
  register_vector<::std::vector<::fd::vertex>>("NeighbourVector");

  class_<::fd::graph>("fdGraph")
    .property("p", &::fd::graph::p)
    .property("c", &::fd::graph::c)
    .property("d", &::fd::graph::d);

  class_<::graph>("graph")
    .class_function("add_point",  &::graph::add_point)
    .class_function("connect",    &::graph::connect)
    .class_function("disconnect", &::graph::disconnect)
    .class_function("get",        &::graph::get,     allow_raw_pointers())
    .class_function("advance",    &::graph::advance, allow_raw_pointers());
}
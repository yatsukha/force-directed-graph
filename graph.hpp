#pragma once

#include <map>
#include <vector>
#include <random>
#include <string>

#include <emscripten/bind.h>

namespace em = ::emscripten;

// used as a closed namespace
class graph final {
  public:

    using scalar = float;

    struct point {
      scalar x;
      scalar y;
    };

  private:

    static scalar random() {
      ::std::mt19937 static mt{::std::random_device{}()};
      ::std::uniform_real_distribution<scalar> static dist{
        static_cast<scalar>(-1),
        static_cast<scalar>(1)
      };

      return dist(mt);
    }

    using range = point;

    range constexpr static x_range = {
      static_cast<scalar>(-1), static_cast<scalar>(1)};

    range constexpr static y_range = {
      static_cast<scalar>(-1), static_cast<scalar>(1)};

    graph() = default;

  public:

    ::std::map<::std::string, point> static points;
    ::std::map<::std::string, ::std::vector<::std::string>> static connections;
      
    static void connect(::std::string const u, ::std::string const v) {
      if (points.find(u) == ::std::end(points))
        points[u] = {random(), random()};

      if (points.find(v) == ::std::end(points))
        points[v] = {random(), random()};

      connections[u].push_back(v);
      connections[v].push_back(u);
    }

    static void advance() {
      
    }

};

using namespace ::em;

EMSCRIPTEN_BINDINGS(graph_binding) {

  value_object<::graph::point>("point")
    .field("x", &::graph::point::x)
    .field("y", &::graph::point::y);

  class_<::graph>("graph")
    .class_property("points", &::graph::points)
    .class_property("connections", &::graph::connections)
    .class_function("connect", &::graph::connect);

  register_vector<::std::string>("VectorString");

  register_map<::std::string, ::graph::point>("MapStringPoint");
  register_map<::std::string, ::std::vector<::std::string>>("MapStringVector");

}
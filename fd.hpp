#pragma once

#include <utility>
#include <vector>
#include <cmath>
#include <functional>
#include <iostream>

namespace fd {

  using unit   = double;
  using vertex = ::std::size_t;
  using point  = ::std::pair<unit, unit>;

  using positions   = ::std::vector<point>;
  using connections = ::std::vector<::std::vector<vertex>>;

  using dimensions = point;

  struct graph {
    positions   p;
    connections c;
    dimensions  d;
  };

  namespace detail {

    inline unit att(unit x, unit k) noexcept {
      return x * x / k;
    }

    inline unit rep(unit x, unit k) noexcept {
      return k * k / x;
    }

    inline unit norm(point const& p) noexcept {
      return ::std::sqrt(p.first * p.first + p.second * p.second);
    }

    inline point operator+(point const& a, point const& b) noexcept {
      return {a.first + b.first, a.second + b.second};
    }

    inline point operator-(point const& p) noexcept {
      return {-p.first, -p.second};
    }

    inline point operator-(point const& a, point const& b) noexcept {
      return {a.first - b.first, a.second - b.second};
    }

    inline point operator*(point const& p, unit const d) noexcept {
      return {p.first * d, p.second * d};
    }

    inline point operator/(point const& p, unit const d) noexcept {
      return {p.first / d, p.second / d};
    }

    // compound

    inline point& operator+=(point& a, point const& b) noexcept {
      a.first  += b.first;
      a.second += b.second;
      return a;
    }

    inline point& operator-=(point& a, point const& b) noexcept {
      a.first  -= b.first;
      a.second -= b.second;
      return a;
    }

    //

    ::std::ostream& operator<<(::std::ostream& out, point const& p) {
      out << "(" << p.first << ", " << p.second << ")";
      return out;
    }

  }

  // Fruchterman-Reingold algorithm for force directed graph drawing.
  inline graph advance(graph g, unit const max_disp) noexcept {
    using namespace detail;

    auto& [pos, con, dimensions] = g;
    auto const [w, h] = dimensions;

    ::std::vector<point> disp(pos.size(), {0, 0});

    auto k = ::std::sqrt(w * h / pos.size());

    for (auto i = 0; i < pos.size() - 1; ++i)
      for (auto j = i + 1; j < pos.size(); ++j) {
        auto delta = pos[i] - pos[j];
        auto ad    = norm(delta);
        auto diff  = delta / ad * rep(ad, k);

        disp[i] += diff;
        disp[j] -= diff;
      }

    for (auto i = 0; i < con.size(); ++i)
      for (auto j : con[i]) {
        auto delta = pos[i] - pos[j];
        auto ad    = norm(delta);
        auto diff  = delta / ad * att(ad, k);

        disp[i] -= diff;
      }

    for (auto i = 0; i < disp.size(); ++i) {
      pos[i] += disp[i] / norm(disp[i]) * ::std::min(max_disp, norm(disp[i]));
      pos[i].first  = ::std::min(w / 2, ::std::max(-w / 2, pos[i].first));
      pos[i].second = ::std::min(h / 2, ::std::max(-h / 2, pos[i].second));
    }

    return g;
  }

}
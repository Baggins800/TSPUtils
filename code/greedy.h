#ifndef GREEDY_H
#define GREEDY_H

#include <vector>
#include <map>
#include <limits>

#include "solver.h"

using std::map;
using std::vector;


class Greedy : public Solver {
 public:
  Greedy(const Graph * g) : Solver(g) {
    best_objective = numeric_limits<double>::max();
  }

  void solve() {
    const Solution *best_sol = nullptr;
    for (auto v : G->get_vertices()) {
      const Solution * s = generate_solution(v);
      if (s != nullptr) {
        if (s->get_objective() < best_objective) {
          best_objective = s->get_objective();
          delete best_sol;
          best_sol = s;
        } else delete s;
      }
    }
    if (best_sol != nullptr) solutions.push_back(best_sol);
  }

 private:
  const Solution * generate_solution(const Vertex * v) {
    vector<const Vertex *> visited;
    map<const Vertex *, bool> visited_map;
    while (visited.size() < G->get_vertices().size()) {
      visited.push_back(v);
      visited_map[v] = true;
      auto a = G->get_min_adjacent(v, visited_map);
      if (a == nullptr) break;
      else v = a->target;
    }
    Solution *s = new Solution(visited, G);
    if (!s->is_feasible()) {
      delete s;
      return nullptr;
    }
    return s;
  }
  double best_objective;
};

#endif

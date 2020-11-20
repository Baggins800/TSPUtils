#ifndef SOLVER_H
#define SOLVER_H
#include <vector>
#include "utils.h"

using std::vector;

class Solver {

 public:
  Solver(const Graph * g) : G(g) {
    solutions.clear();
  }
  virtual void solve() = 0;
  const vector<const Solution *> & get_solutions() const {
    return solutions;
  }

 protected:
  const Graph * G;
  vector<const Solution *> solutions;
};

#endif

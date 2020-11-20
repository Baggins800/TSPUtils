#include <iostream>
#include "utils.h"
#include "argparser.h"
#include "solver.h"
#include "greedy.h"
using std::cout;
using std::endl;
using std::fixed;

// linux: g++ main.cpp && ./a.out --json=pathtosome.json
// windows 10 powershell:  g++ main.cpp && ./a.exe --json=pathtosome.json

int main(int argc, char *argv[]) {
  ArgParser p(argc, argv);
  /*
   * Without arguments, remove ArgParser and use:
   *
   * #include <fstream>
   * ...
   * std::ifstream i("pathtosome.json");
   * json data;
   * i >> data;
   * Graph G(data);
   */
  Graph G(p.get_data());
  //Solution s(G.get_vertices(), &G);
  //G.print_vertices(s.get_solution());
  //cout << fixed << s.get_objective() << endl;
  //cout << s.is_feasible() << endl;


  /* 
   * How to get adjacent and incident vertices:
   *
   * get_adjacent_vertices for a vertex by id or pointer, example with an id:
   *
   *  G.print_vertices(G.get_adjacent_vertices(2220));
   *   or 
   *  G.print_vertices(G.get_adjacent_vertices(some_vertex->id));
   *  same applies to get_incident_vertices(...)
   *
   * example with containers: 
   *  for (auto v : G.get_vertices()) {
   *    G.print_vertices(G.get_adjacent_vertices(v));
   *    G.print_vertices(G.get_incident_vertices(v));
   *  }
   */

  Solver * solver = nullptr;
  /* If you are not using the argument parser,
   * do not use p.use_greedy, p.use_tabu, p.max_iterations, etc.
   * see greedy.h for an example of a solver implementation
   */

  if (p.use_greedy() && (solver == nullptr)) {
    solver = new Greedy(&G);
  }

  //if (p.use_tabu() && (solver == nullptr))
  //  solver = new TabuSearch(G, p.max_iterations(), ...);

  //if (p.use_pso() && (solver == nullptr))
  //  solver = new PSO(G, p.max_iterations(), ...);


  // check if a solver was selected
  if (solver == nullptr) {
    assert(solver);
    exit(127);
  }
  solver->solve();

  // print solutions obtained
  for (auto sol : solver->get_solutions()) {
    G.print_vertices(sol->get_solution());
    cout << "objective value: " << fixed << sol->get_objective() << endl;
  }
  
  delete solver;
  return 0;
}

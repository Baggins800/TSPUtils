#ifndef UTILS_H
#define UTILS_H
#include <vector>
#include <map>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <set>
#include <fstream>
#include <iomanip>
#include <string>
#include "json.hpp"
using std::vector;
using std::string;
using std::set;
using std::iota;
using std::begin;
using std::end;
using std::map;
using std::cout;
using std::endl;
using std::numeric_limits;
using std::isinf;
using std::mt19937_64;
using std::seed_seq;
using std::uniform_real_distribution;
using std::uniform_int_distribution;
using std::sort;
using std::setw;
using std::ofstream;
using json = nlohmann::json;

struct Vertex {
  uint32_t id;
};

struct Arc {
  uint32_t id;
  double weight = 0.0;
  const Vertex * source = nullptr;
  const Vertex * target = nullptr;
};

class Graph {
 public:
  Graph(const json & data) {
    for (auto v : data["vertices"]) {
      Vertex * vert = new Vertex;
      vert->id = v["id"];
      vertex_map[v["id"]] = vert;
      vertices.push_back(vert);
    }
    for (auto a : data["arcs"]) {
      Arc * arc = new Arc;
      arc->weight = a["weights"][0]["value"];
      arc->source = vertex_map[a["source_id"]];
      arc->target = vertex_map[a["target_id"]];
      arc->id = a["id"];
      arcs.push_back(arc);
      arc_map[arc->id] = arc;
    }
  }

  Graph(const uint32_t & seed, const uint32_t & V,
    uint32_t A, const double & min, const double & max) {
    mt19937_64 generator(seed);
    cout << "Generating graph" << endl;
    uniform_real_distribution<double> dist_gen(min, max);
    uniform_int_distribution<uint32_t> vert_gen(0, V - 1);

    for (uint32_t i = 0; i < V; ++i) add_vertex(i);
    sort(vertices.begin(), vertices.end());
    auto init_source = vertices[vert_gen(generator)];
    auto source = init_source;
    auto target = vertices[vert_gen(generator)];
    while (source == target)
      target = vertices[vert_gen(generator)];
    vector<const Vertex *> explored;
    vector<const Vertex *> diff(V);
    explored.push_back(source);

    // generate at least a graph with one solution
    while (explored.size() < V) {
      double distance = dist_gen(generator);
      explored.push_back(target);
      sort(explored.begin(), explored.end());
      add_arc(source, target, distance); 
      source = target;
      auto it = set_difference(vertices.begin(), vertices.end(),
        explored.begin(), explored.end(), diff.begin());
      uniform_int_distribution<uint32_t> rem_gen(0, it - diff.begin());
      target = *(diff.begin());
    }
    add_arc(target, init_source, dist_gen(generator));
    if (A > V * V - V) A = V * V - V;
    while (arcs.size() < A) {
      const Vertex * s = vertices[vert_gen(generator)];
      const Vertex * t = vertices[vert_gen(generator)];
      if (s != t)
        add_arc(s, t, dist_gen(generator));
    }
  }
  void add_vertex(const uint32_t & id) {
    auto vertex = new Vertex;
    vertex->id = id;
    for (auto v : vertices) {
      if (v->id == id) {
        delete vertex;
        assert(false);
        return;
      }
    }
    vertices.push_back(vertex);
  }

  void save_graph_as_json(const string & filename) {
    json jgraph;
    jgraph["vertices"] = {}; 
    jgraph["arcs"] = {}; 
    for (auto v : vertices) {
      json vertex;
      vertex["id"] = v->id;
      jgraph["vertices"].push_back(vertex);
    }
    for (auto a : arcs) {
      json arc;
      arc["id"] = a->id;
      arc["source_id"] = a->source->id;
      arc["target_id"] = a->target->id;
      json weight;
      weight["name"] ="distance";
      weight["value"] = a->weight;
      arc["weights"].push_back(weight);
      jgraph["arcs"].push_back(arc);
    }
    ofstream o(filename);
    o << setw(2) << jgraph << endl;
  }

  void add_arc(const Vertex *s, const Vertex *t, const double & weight) {
    bool exists = false;
    for (auto & a : arcs) {
      if ((a->source == s) && (a->target == t)) {
        exists = true;
        break;
      }
    }
    if (!exists) {
      auto a = new Arc;
      a->id = arcs.size();
      a->source = s;
      a->target = t;
      a->weight = weight;
      arcs.push_back(a);
    }
  }

  vector<const Vertex *> get_adjacent_vertices(const uint32_t & v_id) const {
    vector<const Vertex *> results;
    for (auto a : arcs) {
      if (a->source->id == v_id) results.push_back(a->target);
    }
    return results;
  }

  vector<const Vertex *> get_adjacent_vertices(const Vertex * v) const {
    vector<const Vertex *> results;
    for (auto a : arcs) {
      if (a->source == v) results.push_back(a->target);
    }
    return results;
  }

  vector<const Arc *> get_adjacent_arcs(const Vertex * v) const {
    vector<const Arc *> results;
    for (auto a : arcs) {
      if (a->source == v) results.push_back(a);
    }
    return results;
  }

  vector<const Arc *> get_adjacent_arcs(const uint32_t & v_id) const {
    vector<const Arc *> results;
    for (auto a : arcs) {
      if (a->source->id == v_id) results.push_back(a);
    }
    return results;
  }

  const Arc * get_min_adjacent(const Vertex * v, const map<const Vertex *, bool> & visited) const {
    double min_dist = numeric_limits<double>::max();
    const Arc * result = nullptr;
    for (auto & a : get_adjacent_arcs(v)) {
      if ((a->weight < min_dist) && (visited.find(a->target) == visited.end())) {
        min_dist = a->weight;
        result = a;
      }
    }
    return result;
  }

  vector<const Arc *> get_incident_arcs(const Vertex * v) const {
    vector<const Arc *> results;
    for (auto a : arcs) {
      if (a->target == v) results.push_back(a);
    }
    return results;
  }

  vector<const Arc *> get_incident_arcs(const uint32_t & v_id) const {
    vector<const Arc *> results;
    for (auto a : arcs) {
      if (a->target->id == v_id) results.push_back(a);
    }
    return results;
  }

  vector<const Vertex *> get_incident_vertices(const uint32_t & v_id) const {
    vector<const Vertex *> results;
    for (auto a : arcs) {
      if (a->target->id == v_id) results.push_back(a->source);
    }
    return results;
  }

  vector<const Vertex *> get_incident_vertices(const Vertex * v) const {
    vector<const Vertex *> results;
    for (auto a : arcs) {
      if (a->target == v) results.push_back(a->source);
    }
    return results;
  }

  void print_vertices(const vector<const Vertex *> & V) const {
    cout << "Vertices V in graph G=(V,A)" << endl;
    for (auto v : V) {
      cout <<"id: " << v->id << endl;
    }
  }

  const vector<const Vertex *> get_vertices() const {
    return vertices;
  }

  const double & get_distance(const Arc * arc) const {
    return arc_map.at(arc->id)->weight;
  }

  const double & get_distance(const uint32_t & id) const {
    return arc_map.at(id)->weight;
  }

  const double get_distance(const uint32_t & s_id,
    const uint32_t & t_id) const {
    for (auto a : arcs)
      if (a->source->id == s_id && a->target->id == t_id) return a->weight;
    return numeric_limits<double>::infinity();
  }

  const double get_distance(const Vertex * s, const Vertex * t) const {
    for (auto a : arcs)
      if ((a->source == s) && (a->target == t)) return a->weight;
    return numeric_limits<double>::infinity();
  }

  ~Graph() {
    for (auto &v : vertices) delete v;
    vertices.clear();
    for (auto &a : arcs) delete a;
    arcs.clear();
  }
 private:
  vector<const Vertex *> vertices;
  vector<const Arc *> arcs;
  map<uint32_t, const Vertex *> vertex_map;
  map<uint32_t, const Arc *> arc_map;
};

class Solution {
 public:
  Solution(const vector<const Vertex *> & sol, const Graph * g) {
    feasible = false;
    graph = g;
    for (auto v : sol) {
      solution.push_back(v);
    }
    if (solution.back() != solution.front()) {
      solution.push_back(solution.front());
    }
    if (solution.size() - 1 != g->get_vertices().size()) return;
    calculate_objective();
  }

  void swap_vertices(const uint32_t & pos1, const uint32_t & pos2) {
    if ((pos1 > solution.size() - 1) || (pos2 > solution.size() - 1))
      assert(false);
    const Vertex * t = solution[pos1];
    solution[pos1] = solution[pos2];
    solution[pos2] = t;
    calculate_objective();
  }

  void print() const {
    graph->print_vertices(solution);
  }

  void calculate_objective() {
    bool first = true;
    objective_value = 0.;
    for (auto &v : solution) {
      if (first) {
        first = false;
        objective_value += graph->get_distance(v, *(&v + 1));

      } else {
        if (v != solution.back())
          objective_value += graph->get_distance(v, *(&v + 1));
      }
    }
    feasible = !isinf(objective_value);
  }

  const double & get_objective() const {
    return objective_value;
  }
  
  const bool & is_feasible() const {
    return feasible;
  }

  const vector<const Vertex*> & get_solution() const {
    return solution;
  }

 private:
  vector<const Vertex *> solution;
  double objective_value;
  const Graph * graph;
  bool feasible;
};
#endif

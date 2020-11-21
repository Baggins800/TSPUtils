#ifndef ARGPARSER_H
#define ARGPARSER_H
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include "json.hpp"
using std::map;
using std::string;
using std::vector;
using std::stringstream;
using std::endl;
using std::cout;
using std::left;
using std::setw;
using std::ifstream;
using std::stoi;
using json = nlohmann::json;

class ArgParser {
 public:
  ArgParser(const int & argc, char ** argv) {
    iterations = 10;
    save_filename = "";
    vertices_to_generate = 10;
    arcs_to_generate = 10;
    seed = 0;
    greedy = false;
    tabu = false;
    pso = false;
    for (int i = 1; i < argc; ++i) {
      vector<string> argument_parameters;
      string argument;
      stringstream stream(argv[i]);
      string word;
      while (getline(stream, word, '=')) {
        argument = word;
        while (getline(stream, word, ','))
         argument_parameters.push_back(word);
      } 
      if (argument_parameters.size() == 0)
        argument_parameters.push_back("1");
      this->arguments[argument] = argument_parameters;
    }
    parse_arguments();
  }
  const json & get_data() const {
    return data;
  }
  const bool & use_pso() const { return pso; }
  const bool & use_greedy() const { return greedy; }
  const bool & use_tabu() const { return tabu; }
  const uint32_t & max_iterations() const { return iterations; }
  const uint32_t & max_size() const { return size; }
  const uint32_t & get_seed() const { return seed; }
  const string & get_save_filename() const { return save_filename; }
  const uint32_t & get_vertices_to_generate() const {
    return vertices_to_generate;
  }
  const uint32_t & get_arcs_to_generate() const {
    return arcs_to_generate;
  }
 protected:
  virtual void parse_arguments() {
    help["--load-json"] = "Path to the json file.";
    for (auto p : this->parameters("--load-json")) {
      ifstream i(p);
      i >> data;
      break; 
    }

    help["--save-json"] = "Save a generated graph as a json file.";
    for (auto p : this->parameters("--save-json")) {
      save_filename = p;
      break; 
    }

    help["--iter"] = "The maximumn number of iterations the solver can perform.";
    for (auto p : this->parameters("--iter")) {
      iterations = stoi(p);
      break;
    }
    help["--size"] = "Population/particle/tabu_list size.";
    for (auto p : this->parameters("--size")) {
      size = stoi(p);
      break;
    }

    help["--greedy"] = "Use the greedy solver.";
    for (auto p : this->parameters("--greedy")) {
      greedy = (p != "0");
      break;
    }

    help["--tabu"] = "Use tabu search implementation.";
    for (auto p : this->parameters("--tabu")) {
      tabu = (p != "0");
      break;
    }

    help["--pso"] = "Use PSO implementation.";
    for (auto p : this->parameters("--pso")) {
      pso = (p != "0");
      break;
    }

    help["--seed"] = "Sets the seed to generate a random dataset.";
    for (auto p : this->parameters("--seed")) {
      seed = stoi(p);
      break;
    }

    help["--vertices"] = "Sets number of vertices that needs to be generated.";
    for (auto p : this->parameters("--vertices")) {
      vertices_to_generate = stoi(p);
      break;
    }

    help["--arcs"] = "Sets number of arcs that needs to be generated.";
    for (auto p : this->parameters("--arcs")) {
      arcs_to_generate = stoi(p);
      break;
    }

    for (auto p : this->parameters("--help")) {
      print_help(); break;
    }

  }
  const vector<string> & parameters(const string & s) const {
    if (this->arguments.find(s) != this->arguments.end())
      return this->arguments.at(s);
    return this->empty;
  }
  void print_help() {
    char max_len = 0;
    for (auto h : this->help) {
      if (max_len < h.first.size()) {
        max_len = h.first.size();
      }
    }
    max_len += 3;
    cout << "Arguments: " << endl;
    for (auto h : this->help) {
      cout << "   " << left << setw(max_len) << h.first << " " << h.second << endl;
    }
    exit(0);
  }
  map<string, string> help;
 private:
  map<string, vector<string>> arguments;
  vector<string> empty;
  json data; 
  string save_filename;
  bool greedy;
  bool pso;
  bool tabu;
  uint32_t seed;
  uint32_t iterations;
  uint32_t size;
  uint32_t vertices_to_generate;
  uint32_t arcs_to_generate;
};
#endif

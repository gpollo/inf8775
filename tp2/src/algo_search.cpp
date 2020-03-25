#include <algo.hpp>
#include <algo_search.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <iostream>
#include <cstdlib>
#include <utility>
#include <vector>

namespace tp {

algo_search::algo_search(notes n, transitions t, unsigned int iterations) : algo(n, t), algo_greedy_(n, t), iterations_(iterations) {}

std::pair<unsigned int, std::vector<unsigned int>> algo_search::run() {
  if (notes_.get_values().size() < 2) {
    return std::make_pair(0, std::vector<unsigned int>());
  }

  auto [best_cost, best_fingers] = algo_greedy_.run();
  std::cout << best_cost << " " << best_fingers.size() << std::endl;
  
  for (unsigned int i = 0; i < iterations_; i++) {
//    auto [next_cost, next_fingers] = run(best_fingers, rand() % (best_fingers.size() / 2 - 2) + 1);
    auto [next_cost, next_fingers] = run(best_fingers, best_fingers.size()-1);

    std::cout << next_cost << std::endl;
    if (next_cost <= best_cost) {
      best_cost = next_cost;
      best_fingers = next_fingers;

      std::cout << next_cost << " " << next_fingers.size() << std::endl;
    }
  }

  return std::make_pair(best_cost, best_fingers);
}

std::pair<unsigned int, std::vector<unsigned int>> algo_search::run(std::vector<unsigned int>& fingers, unsigned int until) {
  if (fingers.size() - 1 < until || until < 1) {
    throw std::runtime_error("invalid slice of fingers given");
  }

  if (fingers.size() < notes_.size() - 1) {
    throw std::runtime_error("fingers vector should be at least as large as the number of notes");
  }

  unsigned int n1 = *notes_[0];
  unsigned int n2;
  unsigned int f1 = fingers[0];
  unsigned int f2;

  unsigned int total_cost = 0;
  std::vector<unsigned int> results;

  for (unsigned int i = 1; i < fingers.size(); i++) {
    results.push_back(f1);

    n2 = *notes_[i];
    f2 = fingers[i];
    total_cost += transitions_.cost(n1, f1, n2, f2);
    n1 = n2;
    f1 = f2;
  }

  results.push_back(f1);

  n2 = *notes_[fingers.size()];
  f2 = rand() % transitions::k_finger_count;
  total_cost += transitions_.cost(n1, f1, n2, f2);
  n1 = n2;
  f1 = f2;
  std::cout << f1 << std::endl;

  for (unsigned int i = fingers.size() + 1; i < notes_.size(); i++) {
    results.push_back(f1);
std::cout << "tabarnak" << std::endl;
    n2 = *notes_[i];
    f2 = transitions_.best(n1, f1, n2);
    total_cost += transitions_.cost(n1, f1, n2, f2);
    n1 = n2;
    f1 = f2;
  }

  results.push_back(f1);
  
  return std::make_pair(total_cost, results);
}

} /* namespace tp */

#include <algo.hpp>
#include <algo_greedy.hpp>
#include <notes.hpp>
#include <transitions.hpp>

#include <utility>
#include <vector>

namespace tp {

algo_greedy::algo_greedy(notes n, transitions t) : algo(n, t) {}

std::pair<unsigned int, std::vector<unsigned int>> algo_greedy::run() {
  if (notes_.size() < 2) {
    return std::make_pair(0, std::vector<unsigned int>());
  }

  unsigned int n1 = *notes_[0];
  unsigned int n2 = *notes_[1];
  auto [f1, f2] = transitions_.best(n1, n2);

  unsigned int total_cost = transitions_.cost(n1, f1, n2, f2);
  std::vector<unsigned int> fingers;

  fingers.push_back(f1);
  for (unsigned int i = 2; i < notes_.size(); i++) {
    fingers.push_back(f2);

    n1 = n2;
    n2 = *notes_[i];
    f1 = f2;
    f2 = transitions_.best(n1, f1, n2);
    total_cost += transitions_.cost(n1, f1, n2, f2);
  }
  fingers.push_back(f2);

  return std::make_pair(total_cost, fingers);
}

} /* namespace tp */

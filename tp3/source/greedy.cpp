#include <greedy.hpp>

#include <algorithm>

namespace tp {

greedy::greedy(settings& settings, const population& pop)
  : settings_(settings), population_(pop) {}

type::relations greedy::run() const {
  type::relations isolations;

  for (auto i = 0; i < population_.size(); i++) {
    auto infected = population_.infected(i);
    if (infected == nullptr) {
      continue;
    }

    if (infected->size() < settings_.virality()) {
      continue;
    }

    unsigned int removed = 0;
    for (auto j : *infected) {
      if (j < i) {
        isolations.emplace(j, i);
      } else {
        isolations.emplace(i, j);
      }

      removed++;

      if (infected->size() - removed < settings_.virality()) {
        break;
      }
    }
  }

  return isolations;
}



} /* namespace tp */

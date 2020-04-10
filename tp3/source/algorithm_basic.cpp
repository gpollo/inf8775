#include <algorithm_basic.hpp>

#include <algorithm>
#include <deque>
#include <set>

namespace tp {

algorithm_basic::algorithm_basic(settings& settings, const population& pop)
  : settings_(settings), population_(pop) {}

type::relations algorithm_basic::isolate_infected() const {
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

type::relations algorithm_basic::isolate_50_percent() const {
  type::relations isolations;

  std::set<type::person> everybody;
  for (auto i = 0; i < population_.size(); i++) {
    everybody.insert(i);
  }


  std::set<type::person> all_visited;
  std::set<type::person> good_visited;
  std::deque<type::person> not_visited;

  not_visited.push_back(0);
  while (true) {
    if (not_visited.size() == 0) {
      std::deque<type::person> remaining;
      std::set_difference(
        everybody.begin(), everybody.end(),
        all_visited.begin(), all_visited.end(),
        std::inserter(remaining, remaining.begin())
      );
      not_visited.push_back(remaining.front());
    }

    if (((float) good_visited.size()) / ((float) population_.size()) > 0.60) {
      break;
    }

    auto current = not_visited.front();
    not_visited.pop_front();
    all_visited.insert(current);

    if (population_.infected().contains(current)) {
      continue;
    } else {
      good_visited.insert(current);
    }

    auto relations = population_.relations(current);
    if (relations == nullptr) {
      continue;
    }

    for (const auto other : *relations) {
      not_visited.push_back(other);
    }
  }

  for (const auto i : good_visited) {
    auto relations = population_.relations(i);
    if (relations == nullptr) {
      continue;
    }

    std::deque<type::person> outside;
    std::set_difference(
      relations->begin(), relations->end(),
      good_visited.begin(), good_visited.end(),
      std::inserter(outside, outside.begin())
    );

    unsigned removed_relation_count = 0;
    for (const auto j : outside) {
      if (outside.size() - removed_relation_count < settings_.virality()) {
        break;
      }

      if (good_visited.contains(j)) {
        continue;
      }

      removed_relation_count++;
      if (j < i) {
        isolations.emplace(j, i);
      } else {
        isolations.emplace(i, j);
      }
    }
  }

  return isolations;
}

} /* namespace tp */

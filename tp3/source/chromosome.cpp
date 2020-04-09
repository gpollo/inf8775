#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <algorithm>
#include <optional>
#include <set>

#include <iostream>

namespace tp {

chromosome::chromosome(settings& settings, const population& pop)
  : settings_(settings), population_(pop), cached_(false), cached_cost_(0) {

  std::vector<std::pair<unsigned int, unsigned int>> relations;
  relations.insert(relations.begin(), population_.relations().begin(), population_.relations().end());

  int isolation_count = relations.size() * settings_.initial_isolation_factor();
  while (isolations_.size() < isolation_count) {
    unsigned int pos = settings_.random_to(relations.size()-1);
    isolations_.insert(relations[pos]);
  }
}

chromosome::chromosome(settings& settings, const population& pop, const std::set<std::pair<unsigned int, unsigned int>>& isol)
  : settings_(settings), population_(pop), isolations_(isol), cached_(false), cached_cost_(0) {}

const std::set<std::pair<unsigned int, unsigned int>>& chromosome::isolations() const {
  return isolations_;
}

std::pair<chromosome*, chromosome*> chromosome::cross(const chromosome* other) const {
  std::vector<std::pair<unsigned int, unsigned int>> combined;

  for (const auto& isolation : isolations_) {
    combined.push_back(isolation);
  }

  for (const auto& isolation : other->isolations_) {
    combined.push_back(isolation);
  }

  std::set<std::pair<unsigned int, unsigned int>> start;
  chromosome* child1 = new chromosome(settings_, population_, start);
  chromosome* child2 = new chromosome(settings_, population_, start);

  for (const auto& isolation : combined) {
    if (child1->isolations_.find(isolation) != child1->isolations_.end()) {
       child2->isolations_.insert(isolation);
       continue;
    }

    if (child2->isolations_.find(isolation) != child2->isolations_.end()) {
       child1->isolations_.insert(isolation);
       continue;
    }

    if (settings_.binary_random()) {
      child1->isolations_.insert(isolation);
    } else {
      child2->isolations_.insert(isolation);
    }
  }

  return std::make_pair(child1, child2);
}

chromosome* chromosome::mutate(unsigned int add, unsigned int remove, unsigned int update) const {
  std::set<std::pair<unsigned int, unsigned int>> start;
  chromosome* mutation = new chromosome(settings_, population_, start);
  mutation->isolations_.insert(isolations_.begin(), isolations_.end());

  for (int i = 0; i < (int) add; i++) {
    mutation->add_isolation();
  }

  for (int i = 0; i < (int) remove; i++) {
    mutation->remove_isolation();
  }

  for (int i = 0; i < (int) update; i++) {
    mutation->update_isolation();
  }

  return mutation;
}

std::optional<unsigned int> chromosome::cost() {
  float infected_percent = population_.run(settings_.virality(), isolations_);
  if (infected_percent > 50) {
    return {};
  }
    
  return isolations_.size();
}

void chromosome::add_isolation() {
  auto r = population_.relations();
  auto i = isolations_;

  std::vector<std::pair<unsigned int, unsigned int>> available;
  set_difference(r.begin(), r.end(), i.begin(), i.end(), std::inserter(available, available.begin()));
  if (available.size() != 0) {
    isolations_.insert(available[settings_.random_to(available.size() - 1)]);
  }
}

void chromosome::remove_isolation() {
  if (isolations_.size() <= 1) {
    return;
  }

  auto it = isolations_.begin();
  std::advance(it, settings_.random_to(isolations_.size() - 1));
  isolations_.erase(it);
}

void chromosome::update_isolation() {
  remove_isolation();
  add_isolation();
}

} /* namespace tp */

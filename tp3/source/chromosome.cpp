#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <algorithm>
#include <set>

namespace tp {

chromosome::chromosome(settings& settings, const population& pop)
  : settings_(settings), population_(pop) {

  std::vector<std::pair<unsigned int, unsigned int>> relations;
  relations.insert(relations.begin(), population_.relations().begin(), population_.relations().end());

  int isolation_count = relations.size() * settings_.initial_isolation_factor();
  while (isolations_.size() < isolation_count) {
    unsigned int pos = settings_.random_to(relations.size()-1);
    isolations_.insert(relations[pos]);
  }
}

chromosome::chromosome(settings& settings, const population& pop, const std::set<std::pair<unsigned int, unsigned int>>& isol)
  : settings_(settings), population_(pop), isolations_(isol) {}

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
    if (settings_.binary_random()) {
      child1->isolations_.insert(isolation);
    } else {
      child2->isolations_.insert(isolation);
    }
  }

  return std::make_pair(child1, child2);
}

chromosome* chromosome::mutate() const {
  std::set<std::pair<unsigned int, unsigned int>> start;
  chromosome* mutation = new chromosome(settings_, population_, start);
  for (const auto& isolation : isolations_) {
    mutation->isolations_.insert(isolation);
  }

  for (unsigned int i = 0; i < settings_.isolation_mutation_count(); i++) {
    unsigned int random = settings_.percent_random();
    if (random < 33) {
      mutation->add_isolation();
    } else if (random < 66) {
      mutation->remove_isolation();
    } else {
      mutation->update_isolation();
    }
  }

  return mutation;
}

unsigned int chromosome::cost() const {
  tp::population pop = population_;
  for (const auto& isolation : isolations_) {
    pop.remove_relation(isolation.first, isolation.second);
  }

  while (pop.run_iteration(settings_.virality()) != 0) {}

  float infected_percent = 100 * (((float) pop.infected().size()) / ((float) pop.size()));
  if (infected_percent > 50) {
    return population_.relations().size();
  }

  return isolations_.size();
}

void chromosome::add_isolation() {
  auto r = population_.relations();
  auto i = isolations_;

  std::vector<std::pair<unsigned int, unsigned int>> available;
  set_difference(r.begin(), r.end(), i.begin(), i.end(), std::inserter(available, available.begin()));
  isolations_.insert(available[settings_.random_to(available.size() - 1)]);
}

void chromosome::remove_isolation() {
  auto it = isolations_.begin();
  std::advance(it, settings_.random_to(isolations_.size() - 1));
  isolations_.erase(it);
}

void chromosome::update_isolation() {
  remove_isolation();
  add_isolation();
}

} /* namespace tp */

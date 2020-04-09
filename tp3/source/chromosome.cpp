#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <algorithm>
#include <optional>

namespace tp {

chromosome::chromosome(settings& settings, const population& pop)
  : settings_(settings), population_(pop) {

  const type::relations& relations = population_.relations();
  std::vector<type::relation> relations_vec(relations.begin(), relations.end());

  int isolation_count = relations.size() * settings_.initial_isolation_factor();
  while (isolations_.size() < isolation_count) {
    isolations_.insert(settings_.random_from(relations_vec));
  }
}

chromosome::chromosome(settings& settings, const population& pop, const type::relations& isolations)
  : settings_(settings), population_(pop), isolations_(isolations) {}

const type::relations& chromosome::isolations() const {
  return isolations_;
}

std::pair<chromosome*, chromosome*> chromosome::cross(const chromosome* other) const {
  std::vector<type::relation> combined;
  combined.insert(combined.end(), isolations_.begin(), isolations_.end());
  combined.insert(combined.end(), other->isolations_.begin(), other->isolations_.end());

  chromosome* child1 = new chromosome(settings_, population_, type::relations{});
  chromosome* child2 = new chromosome(settings_, population_, type::relations{});

  for (const auto& isolation : combined) {
    if (child1->isolations_.contains(isolation)) {
       child2->isolations_.insert(isolation);
       continue;
    }

    if (child2->isolations_.contains(isolation)) {
       child1->isolations_.insert(isolation);
       continue;
    }

    if (settings_.binary_random()) {
      child1->isolations_.insert(isolation);
    } else {
      child2->isolations_.insert(isolation);
    }
  }

  return {child1, child2};
}

chromosome* chromosome::mutate(unsigned int add, unsigned int remove, unsigned int update) const {
  chromosome* mutation = new chromosome(settings_, population_, isolations_);

  for (auto i = 0; i < add; i++) {
    mutation->add_isolation();
  }

  for (auto i = 0; i < remove; i++) {
    mutation->remove_isolation();
  }

  for (auto i = 0; i < update; i++) {
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

  std::vector<type::relation> available;
  set_difference(r.begin(), r.end(), i.begin(), i.end(), std::inserter(available, available.begin()));

  if (available.size() != 0) {
    isolations_.insert(settings_.random_from(available));
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

#include <population.hpp>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <variant>

namespace tp {

population::population(unsigned int size) : size_(size) {}

std::variant<population, std::string> population::from_file(const std::filesystem::path& path) {
  std::ifstream file(path, std::ifstream::in);
  if (file.fail()) {
    return strerror(errno);
  }

  unsigned int n, m;
  if (!(file >> n >> m)) {
    return strerror(errno);
  }

  population p(n);

  for (type::person i = 0; i < n; i++) {
    for (type::person j = 0; j < n; j++) {
      unsigned int r;
      if (!(file >> r)) {
        return strerror(errno);
      }

      if (r) {
        p.add_relation({i, j});
      }
    }
  }

  for (auto i = 0; i < m; i++) {
    type::person j;
    if (!(file >> j)) {
      return strerror(errno);
    }

    p.add_infected(j);
  }

  return p;
}

unsigned int population::size() const {
  return size_;
}

void population::add_relation(const type::relation& relation) {
  auto [i, j] = relation;

  if (i == j) {
    return;
  }

  if (j < i) {
    std::swap(i, j);
  }

  if (!all_relations_.emplace(i, j).second) {
    return;
  }

  auto i_it = relations_.find(i);
  if (i_it == relations_.end()) {
    auto inserted = relations_.emplace(i, type::persons{});
    inserted.first->second.insert(j);
  } else {
    i_it->second.insert(j);
  }

  auto j_it = relations_.find(j);
  if (j_it == relations_.end()) {
    auto inserted = relations_.emplace(j, type::persons{});
    inserted.first->second.insert(i);
  } else {
    j_it->second.insert(i);
  }

  update_infected({i, j});
}

void population::add_infected(type::person i) {
  if (all_infected_.insert(i).second) {
    update_infected(i);
  }
}

void population::remove_relation(const type::relation& relation) {
  auto [i, j] = relation;

  if (i == j) {
    return;
  }

  if (j < i) {
    std::swap(i, j);
  }

  auto relations_it_i = relations_.find(i);
  if (relations_it_i != relations_.end()) {
    relations_it_i->second.erase(j);
  }

  auto relations_it_j = relations_.find(j);
  if (relations_it_j != relations_.end()) {
    relations_it_j->second.erase(i);
  }

  auto infected_relations_it_i = infected_relations_.find(i);
  if (infected_relations_it_i != infected_relations_.end()) {
    infected_relations_it_i->second.erase(j);
  }

  auto infected_relations_it_j = infected_relations_.find(j);
  if (infected_relations_it_j != infected_relations_.end()) {
    infected_relations_it_j->second.erase(i);
  }

  all_relations_.erase({i, j});
}

const type::relations& population::relations() const {
  return all_relations_;
}

const type::persons* population::relations(type::person i) const {
  auto it = relations_.find(i);
  if (it == relations_.end()) {
    return nullptr;
  }

  return &it->second;
}

const type::persons& population::infected() const {
  return all_infected_;
}

const type::persons* population::infected(type::person i) const {
  auto it = infected_relations_.find(i);
  if (it == infected_relations_.end()) {
    return nullptr;
  }

  return &it->second;
}

float population::run(unsigned int virality, const type::relations& isolations) const {
  population pop(*this);
  for (const auto& isolation : isolations) {
    pop.remove_relation(isolation);
  }

  while (pop.run_iteration(virality) != 0) {}

  return 100 * (((float) pop.infected().size()) / ((float) pop.size()));
}

unsigned int population::run_iteration(unsigned int virality) {
  type::persons new_cases;

  for (type::person i = 0; i < size_; i++) {
    if (all_infected_.contains(i)) {
      continue;
    }

    auto it = infected_relations_.find(i);
    if (it == infected_relations_.end()) {
      continue;
    }

    if (it->second.size() >= virality) {
      new_cases.insert(i);
    }
  }

  for (auto i : new_cases) {
    add_infected(i);
  }

  return new_cases.size();
}

void population::update_infected(const type::relation& relation) {
  auto [i, j] = relation;

  if (all_infected_.contains(i)) {
    auto it = infected_relations_.find(j);
    if (it == infected_relations_.end()) {
      auto inserted = infected_relations_.emplace(j, type::persons{});
      inserted.first->second.insert(i);
    } else {
      it->second.insert(i);
    }
  }

  if (all_infected_.contains(j)) {
    auto it = infected_relations_.find(i);
    if (it == infected_relations_.end()) {
      auto inserted = infected_relations_.emplace(i, type::persons{});
      inserted.first->second.insert(j);
    } else {
      it->second.insert(j);
    }
  }
}

void population::update_infected(type::person i) {
  auto relations_it = relations_.find(i);
  if (relations_it == relations_.end()) {
    return;
  }

  for (const auto& j : relations_it->second) {
    auto j_it = infected_relations_.find(j);
    if (j_it == infected_relations_.end()) {
      auto inserted = infected_relations_.emplace(j, type::persons{});
      inserted.first->second.insert(i);
    } else {
      j_it->second.insert(i);
    }
  }
}

} /* namespace tp */

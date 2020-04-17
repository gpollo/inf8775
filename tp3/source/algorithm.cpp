#include <algorithm.hpp>
#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <chrono>
#include <iostream>

namespace tp {

algorithm::algorithm(bool print_solutions, bool print_timestamp, settings& settings, const population& pop)
  : print_solutions_(print_solutions), print_timestamp_(print_timestamp), 
    running_(false), settings_(settings), population_(pop) {

  for (auto i = 0; i < settings_.chromosome_count(); i++) {
    chromosomes_.insert(new chromosome(settings_, population_));
  }
}

algorithm::~algorithm() {
  std::for_each(chromosomes_.begin(), chromosomes_.end(), std::default_delete<chromosome>());
}

void algorithm::run() {
  running_ = true;
  start_time_ = std::chrono::high_resolution_clock::now();

  bool best_is_set = false;
  type::solution best;

  while(running_) {
    auto current = evolve();
    if (current.second == nullptr) {
      continue;
    }

    if (!best_is_set) {
      best_is_set = true;
      best = current;
      print_solution(best);
      continue;
    }

    if (current.first < best.first) {
      best = current;
      print_solution(best);
    }
  }
}

void algorithm::print_solution(const type::solution& solution) const {
  if (print_solutions_) {
    std::string output;
    for (auto isolation : solution.second->isolations()) {
      output += std::to_string(isolation.first) + " " + std::to_string(isolation.second) + "\n";
    }
    std::cout << std::endl << output;
  } else if  (print_timestamp_) {
    auto elapsed = std::chrono::high_resolution_clock::now() - start_time_;
    std::cout << solution.first << " " << elapsed.count() << std::endl;
  } else {
    std::cout << solution.first << std::endl;
  }
}

void algorithm::stop() {
  running_ = false;
}

type::solution algorithm::evolve() {
  mutate_random_chromosomes();
  cross_random_chromosomes();

  std::vector<chromosome*> chromosomes_vector(chromosomes_.begin(), chromosomes_.end());
  parallel::chromosome_costs chromosome_costs;
  chromosome_costs(chromosomes_vector, population_.relations().size());

  type::chromosomes removed;
  remove_worst_chromosomes(removed, chromosome_costs.costs());
  replace_invalid_chromosomes(removed, chromosome_costs.invalids());
  
  std::for_each(removed.begin(), removed.end(), std::default_delete<chromosome>());
  if (removed.size() == chromosomes_vector.size()) {
    return {0, nullptr};
  }

  return *chromosome_costs.costs().begin();
}

void algorithm::remove_worst_chromosomes(type::chromosomes& removed, const type::chromosome_costs& costs) {
  auto it = costs.rbegin();
  while (chromosomes_.size() > settings_.chromosome_count()) {
    auto chromosome = (it++)->second;
    removed.insert(chromosome);
    chromosomes_.erase(chromosome);
  }
}

void algorithm::replace_invalid_chromosomes(type::chromosomes& removed, const type::chromosomes& invalids) {
  for (auto invalid : invalids) {
    auto it = chromosomes_.find(invalid);
    if (it == chromosomes_.end()) {
      continue;
    }

    auto chromosome = (*it);
    chromosomes_.erase(it);
    mutate_increase_chromosome(chromosome);
    removed.insert(chromosome);
  }
}


void algorithm::cross_random_chromosomes() {
  if (chromosomes_.size() < 2) {
    return;
  }

  std::vector<parallel::cross_settings> cross_settings;
  for (auto n = 0; n < settings_.cross_count(); n++) {
    auto [i, j] = settings_.random_pair(chromosomes_.size() - 1);
    auto it_i = chromosomes_.begin();
    auto it_j = chromosomes_.begin();
    std::advance(it_i, i);
    std::advance(it_j, j);
    cross_settings.emplace_back(*it_i, *it_j);
  }

  parallel::chromosome_cross chromosome_cross;
  chromosome_cross(cross_settings);

  const auto& c = chromosome_cross.created();
  for_each(c.begin(), c.end(), [&](auto c) { chromosomes_.insert(c); });
}

void algorithm::mutate_random_chromosomes() {
  if (chromosomes_.size() < 1) {
    return;
  }

  std::vector<parallel::mutation_settings> mutation_settings;
  for (auto i = 0; i < settings_.mutation_count(); i++) {
    auto it = chromosomes_.begin();
    std::advance(it, settings_.random_to(chromosomes_.size() - 1));

    unsigned int add = settings_.random_to(10);
    unsigned int remove = settings_.random_to(10);
    unsigned int update = settings_.random_to(10);
    mutation_settings.emplace_back(*it, add, remove, update);
  }

  parallel::chromosome_mutate chromosome_mutate;
  chromosome_mutate(mutation_settings);

  const auto& c = chromosome_mutate.created();
  for_each(c.begin(), c.end(), [&](auto c) { chromosomes_.insert(c); });
}

void algorithm::mutate_increase_chromosome(chromosome* chromosome) {
  unsigned int add = settings_.random_to(20);
  unsigned int remove = 0;
  unsigned int update = 0;
  chromosomes_.insert(chromosome->mutate(add, remove, update));
}

} /* namespace tp */

#include <algorithm.hpp>
#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <population.hpp>
#include <settings.hpp>


#include <iostream>
#include <map>

namespace tp {

algorithm::algorithm(bool print_solutions, settings& settings, const population& pop)
  : print_solutions_(print_solutions), running_(false), settings_(settings), population_(pop) {

  for (unsigned int i = 0; i < settings_.chromosome_count(); i++) {
    chromosomes_.insert(new chromosome(settings_, population_));
  }
}

algorithm::~algorithm() {
  for (auto chromosome : chromosomes_) {
    delete chromosome;
  }
}

void algorithm::run() {
  running_ = true;

  bool best_is_set = false;
  std::pair<unsigned int, chromosome*> best;
  while(running_) {
    auto current = evolve();

    if (!best_is_set) {
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

void algorithm::print_solution(const std::pair<unsigned int, chromosome*>& solution) const {
  if (print_solutions_) {
    for (auto isolation : solution.second->isolations()) {
      std::cout << isolation.first << " " << isolation.second << std::endl;
    }
    std::cout << std::endl;
  } else {
    std::cout << solution.first << std::endl;
  }
}

void algorithm::stop() {
  running_ = false;
}

std::pair<unsigned int, chromosome*> algorithm::evolve() {
  for (unsigned int i = 0; i < settings_.mutation_count(); i++) {
    mutate_random_chromosome();
  }

  for (unsigned int n = 0; n < settings_.cross_count(); n++) {
    cross_random_chromosomes();
  }

  std::vector<chromosome*> chromosomes_vector(chromosomes_.begin(), chromosomes_.end());
  parallel::chromosome_costs chromosome_costs;
  chromosome_costs(chromosomes_vector, population_.relations().size());

  std::set<chromosome*> removed;
  remove_worst_chromosomes(removed, chromosome_costs.costs());
  replace_invalid_chromosomes(removed, chromosome_costs.invalids());
  
  for (auto chromosome : removed) {
    delete chromosome;
  }

  auto best = chromosome_costs.costs().begin();
  return std::make_pair(best->first, best->second);
}

void algorithm::remove_worst_chromosomes(std::set<chromosome*>& removed, const std::multimap<unsigned int, chromosome*>& costs) {
  auto it = costs.rbegin();
  while (chromosomes_.size() > settings_.chromosome_count()) {
    auto chromosome = (it++)->second;
    removed.insert(chromosome);
    chromosomes_.erase(chromosome);
  }
}

void algorithm::replace_invalid_chromosomes(std::set<chromosome*>& removed, std::set<chromosome*> invalids) {
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
  unsigned int n_i = 0, n_j = 0;
  while (n_i != n_j) {
    n_i = settings_.random_to(chromosomes_.size() - 1);
    n_j = settings_.random_to(chromosomes_.size() - 1);
  }

  auto it_i = chromosomes_.begin();
  auto it_j = chromosomes_.begin();
  std::advance(it_i, n_i);
  std::advance(it_j, n_j);
  auto i = *it_i;
  auto j = *it_j;

  auto [n1, n2] = i->cross(j);
  chromosomes_.insert(n1);
  chromosomes_.insert(n2);
}

void algorithm::mutate_random_chromosome() {
  unsigned int n = settings_.random_to(chromosomes_.size() - 1);
  auto it = chromosomes_.begin();
  std::advance(it, n);

  unsigned int add = settings_.random_to(10);
  unsigned int remove = settings_.random_to(10);
  unsigned int update = settings_.random_to(10);
  chromosomes_.insert((*it)->mutate(add, remove, update));
}

void algorithm::mutate_increase_chromosome(chromosome* chromosome) {
  unsigned int add = settings_.random_to(200);
  unsigned int remove = 0;
  unsigned int update = 0;
  chromosomes_.insert(chromosome->mutate(add, remove, update));
}

} /* namespace tp */

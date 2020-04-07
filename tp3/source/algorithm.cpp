#include <algorithm.hpp>
#include <chromosome.hpp>
#include <chromosome_parallel.hpp>
#include <population.hpp>
#include <settings.hpp>


#include <iostream>
#include <map>

namespace tp {

algorithm::algorithm(settings& settings, const population& pop)
  : running_(false), settings_(settings), population_(pop) {

  for (unsigned int i = 0; i < settings_.initial_chromosome_count(); i++) {
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

  while(running_) {
    evolve();
  }
}

void algorithm::stop() {
  running_ = false;
}

void algorithm::evolve() {
  for (unsigned int n = 0; n < settings_.cross_count(); n++) {
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

  for (unsigned int i = 0; i < settings_.mutation_count(); i++) {
    unsigned int n = settings_.random_to(chromosomes_.size() - 1);
    auto it = chromosomes_.begin();
    std::advance(it, n);
    auto c = *it;
    chromosomes_.insert(c->mutate());
  }

  std::vector<chromosome*> chromosomes_vector(chromosomes_.begin(), chromosomes_.end());
  std::multimap<unsigned int, chromosome*> costs;
  parallel::chromosome_costs(chromosomes_vector, costs);

  std::cout << "[";
  for (auto result : costs) {
    std::cout << result.first << " ";
  }
  std::cout << std::endl;

  auto it = costs.rbegin();
  while (chromosomes_.size() > settings_.initial_chromosome_count()) {
    chromosomes_.erase((it++)->second);
  }

  auto best_it = costs.begin();
  std::cout << best_it->first << "|" << best_it->second->isolations().size() << std::endl;
}

} /* namespace tp */

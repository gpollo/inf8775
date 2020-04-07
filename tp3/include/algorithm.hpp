#ifndef INCLUDE_ALGORITHM_HPP
#define INCLUDE_ALGORITHM_HPP

#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <atomic>
#include <set>

namespace tp {

class algorithm {
  public:
    algorithm(bool print_solutions, settings& settings, const population& pop);
    ~algorithm();
    void run();
    void stop();
  private:
    void print_solution(const std::pair<unsigned int, chromosome*>& solution) const;
    std::pair<unsigned int, chromosome*> evolve();
    void remove_worst_chromosomes(std::set<chromosome*>& removed, const std::multimap<unsigned int, chromosome*>& costs);
    void replace_invalid_chromosomes(std::set<chromosome*>& removed, std::set<chromosome*> invalids);

    void cross_random_chromosomes();
    void mutate_random_chromosome();
    void mutate_increase_chromosome(chromosome* chromosome);

    const bool print_solutions_;
    std::atomic_bool running_;
    settings& settings_;
    const population& population_;
    std::set<chromosome*> chromosomes_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGORITHM_HPP */

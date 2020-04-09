#ifndef INCLUDE_ALGORITHM_HPP
#define INCLUDE_ALGORITHM_HPP

#include <chromosome.hpp>
#include <population.hpp>
#include <settings.hpp>

#include <atomic>
#include <chrono>
#include <set>

namespace tp::type {

using solution = std::pair<unsigned int, chromosome*>;
using chromosome_costs = std::multimap<unsigned int, tp::chromosome*>;
using chromosomes = std::set<tp::chromosome*>;

} /* namespace tp::type */

namespace tp {

class algorithm {
  public:
    algorithm(bool print_solutions, bool print_timestamp, settings& settings, const population& pop);
    ~algorithm();
    void run();
    void stop();
  private:
    void print_solution(const type::solution& solution) const;
    type::solution evolve();
    void remove_worst_chromosomes(type::chromosomes& removed, const type::chromosome_costs& costs);
    void replace_invalid_chromosomes(type::chromosomes& removed, const type::chromosomes& invalids);

    void cross_random_chromosomes();
    void mutate_random_chromosomes();
    void mutate_increase_chromosome(chromosome* chromosome);

    const bool print_solutions_;
    const bool print_timestamp_;
    std::atomic_bool running_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;

    settings& settings_;
    const population& population_;
    type::chromosomes chromosomes_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGORITHM_HPP */

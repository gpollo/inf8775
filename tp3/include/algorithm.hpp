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
    algorithm(settings& settings, const population& pop);
    ~algorithm();
    void run();
    void stop();
  private:
    void evolve();

    std::atomic_bool running_;
    settings& settings_;
    const population& population_;
    std::set<chromosome*> chromosomes_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGORITHM_HPP */

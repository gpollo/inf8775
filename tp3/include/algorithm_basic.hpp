#ifndef INCLUDE_ALGORITHM_BASIC_HPP
#define INCLUDE_ALGORITHM_BASIC_HPP

#include <population.hpp>
#include <settings.hpp>

namespace tp {

class algorithm_basic {
  public:
    algorithm_basic(settings& settings, const population& pop);
    type::relations isolate_infected() const;
    type::relations isolate_50_percent() const;
  private:
    const settings& settings_;
    const population& population_;
};

} /* namespace tp */

#endif /* INCLUDE_ALGORITHM_BASIC_HPP */

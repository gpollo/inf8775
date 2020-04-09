#ifndef INCLUDE_GREEDY_HPP
#define INCLUDE_GREEDY_HPP

#include <population.hpp>
#include <settings.hpp>

namespace tp {

class greedy {
  public:
    greedy(settings& settings, const population& pop);
    type::relations run() const;
  private:
    const settings& settings_;
    const population& population_;
};

} /* namespace tp */

#endif /* INCLUDE_GREEDY_HPP */

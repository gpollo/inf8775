#ifndef INCLUDE_CHROMOSOME_HPP
#define INCLUDE_CHROMOSOME_HPP

#include <settings.hpp>
#include <population.hpp>

#include <set>

namespace tp {

class chromosome {
  public:
    chromosome(settings& settings, const population& pop);
    chromosome(settings& settings, const population& pop, const std::set<std::pair<unsigned int, unsigned int>>& isol);

    const std::set<std::pair<unsigned int, unsigned int>>& isolations() const;
    std::pair<chromosome*, chromosome*> cross(const chromosome* other) const;
    chromosome* mutate() const;
    unsigned int cost() const;
  private:
    void add_isolation();
    void remove_isolation();
    void update_isolation();

    settings& settings_;
    const population& population_;
    std::set<std::pair<unsigned int, unsigned int>> isolations_;
};

} /* namespace tp */

#endif /* INCLUDE_CHROMOSOME_HPP */

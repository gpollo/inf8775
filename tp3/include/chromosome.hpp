#ifndef INCLUDE_CHROMOSOME_HPP
#define INCLUDE_CHROMOSOME_HPP

#include <settings.hpp>
#include <population.hpp>

#include <optional>
#include <set>

namespace tp {

class chromosome {
  public:
    chromosome(settings& settings, const population& pop);
    chromosome(settings& settings, const population& pop, const std::set<std::pair<unsigned int, unsigned int>>& isol);

    const std::set<std::pair<unsigned int, unsigned int>>& isolations() const;
    std::pair<chromosome*, chromosome*> cross(const chromosome* other) const;
    chromosome* mutate(unsigned int add, unsigned int remove, unsigned int update) const;
    std::optional<unsigned int> cost();
  private:
    void add_isolation();
    void remove_isolation();
    void update_isolation();

    settings& settings_;
    const population& population_;
    std::set<std::pair<unsigned int, unsigned int>> isolations_;
    bool cached_;
    unsigned int cached_cost_;
};

} /* namespace tp */

#endif /* INCLUDE_CHROMOSOME_HPP */

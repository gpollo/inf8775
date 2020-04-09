#ifndef INCLUDE_CHROMOSOME_HPP
#define INCLUDE_CHROMOSOME_HPP

#include <settings.hpp>
#include <population.hpp>

#include <optional>
#include <set>
#include <map>

namespace tp {

class chromosome {
  public:
    chromosome(settings& settings, const population& pop);
    chromosome(settings& settings, const population& pop, const type::relations& isolations);

    const type::relations& isolations() const;
    std::pair<chromosome*, chromosome*> cross(const chromosome* other) const;
    chromosome* mutate(unsigned int add, unsigned int remove, unsigned int update) const;
    std::optional<unsigned int> cost();
  private:
    void add_isolation();
    void remove_isolation();
    void update_isolation();

    settings& settings_;
    const population& population_;
    type::relations isolations_;
};

} /* namespace tp */

#endif /* INCLUDE_CHROMOSOME_HPP */

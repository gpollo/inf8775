#ifndef INCLUDE_CHROMOSOME_PARALLEL_HPP
#define INCLUDE_CHROMOSOME_PARALLEL_HPP

#include <chromosome.hpp>

#include <map>
#include <vector>

namespace tp::parallel {

void chromosome_costs(std::vector<chromosome*>& c, std::multimap<unsigned int, chromosome*>& r);

} /* namespace tp */

#endif /* INCLUDE_CHROMOSOME_PARALLEL_HPP */

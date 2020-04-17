#include <settings.hpp>

#include <random>
#include <utility>
#include <vector>

namespace tp {

settings::settings(unsigned int virality)
  : initial_isolation_factor_(0.5), chromosome_count_(10),
    virality_(virality),
    cross_count_(10), mutation_count_(100),
    random_device_(), generator_(random_device_()), uniform_(0, 100) {}

bool settings::binary_random() {
  return uniform_(generator_) % 2 == 0;
}

unsigned int settings::percent_random() {
  return uniform_(generator_);
}

unsigned int settings::random_to(unsigned int upper) {
  std::uniform_int_distribution<unsigned int> uniform(0, upper);
  return uniform(generator_);
}

std::pair<unsigned int, unsigned int> settings::random_pair(unsigned max) {
  unsigned int i = 0, j = 0;
  while (i == j) {
    i = random_to(max);
    j = random_to(max);
  }
  
  return {i, j};
}

float settings::initial_isolation_factor() const {
  return initial_isolation_factor_;
}

unsigned int settings::chromosome_count() const {
  return chromosome_count_;
}

unsigned int settings::virality() const {
  return virality_;
}

unsigned int settings::cross_count() const {
  return cross_count_;
}

unsigned int settings::mutation_count() const {
  return mutation_count_;
}

} /* namespace tp */

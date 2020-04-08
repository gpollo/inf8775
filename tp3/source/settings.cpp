#include <settings.hpp>

namespace tp {

settings::settings()
  : initial_isolation_factor_(0.3), chromosome_count_(25),
    virality_(3),
    cross_count_(10), mutation_count_(40),
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

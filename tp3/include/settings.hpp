#ifndef INCLUDE_SETTINGS_HPP
#define INCLUDE_SETTINGS_HPP

#include <random>

namespace tp {

class settings {
  public:
    settings();
    virtual bool binary_random();
    virtual unsigned int percent_random();
    virtual unsigned int random_to(unsigned int upper);

    virtual float initial_isolation_factor() const;
    virtual unsigned int initial_chromosome_count() const;
    virtual unsigned int virality() const;

    virtual unsigned int cross_count() const;
    virtual unsigned int mutation_count() const;
    virtual unsigned int isolation_mutation_count() const;
  protected:
    const float initial_isolation_factor_;
    const unsigned int initial_chromosome_count_;
    const unsigned int virality_;

    const unsigned int cross_count_;
    const unsigned int mutation_count_;
    const unsigned int isolation_mutation_count_;

    std::random_device random_device_;
    std::mt19937 generator_;
    std::uniform_int_distribution<unsigned int> uniform_;
};

} /* namespace tp */

#endif /* INCLUDE_SETTINGS_HPP */

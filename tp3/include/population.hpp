#ifndef INCLUDE_POPULATION_HPP
#define INCLUDE_POPULATION_HPP

#include <filesystem>
#include <map>
#include <set>
#include <variant>

namespace tp::type {

// TODO: maybe use unordered_set or/and unordered_map

using person = unsigned int;
using persons = std::set<person>;
using adjacents = std::map<person, std::set<person>>;
using relation = std::pair<person, person>;
using relations = std::set<relation>;

} /* namespace tp::type */

namespace tp {

class population {
  public:
    population(unsigned int size);
    static std::variant<population, std::string> from_file(const std::filesystem::path& path);
    unsigned int size() const;

    void add_relation(const type::relation& relation);
    void add_infected(type::person i);
    void remove_relation(const type::relation& relation);

    const type::relations& relations() const;
    const type::persons* relations(type::person i) const;

    const type::persons& infected() const;
    const type::persons* infected(type::person i) const;


    float run(unsigned int virality, const type::relations& isolations) const;
    unsigned int run_iteration(unsigned int virality);

  private:
    void update_infected(const type::relation& relation);
    void update_infected(type::person i);

    const unsigned int size_;
    type::adjacents relations_;
    type::adjacents infected_relations_;
    type::relations all_relations_;
    type::persons all_infected_;
};

} /* namespace tp */

#endif /* INCLUDE_POPULATION_HPP */

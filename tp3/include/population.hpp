#ifndef INCLUDE_POPULATION_HPP
#define INCLUDE_POPULATION_HPP

#include <filesystem>
#include <map>
#include <set>
#include <variant>

namespace tp {

class population {
  public:
    population(unsigned int size);
    static std::variant<population, std::string> from_file(const std::filesystem::path& path);
    unsigned int size() const;

    void add_relation(unsigned int i, unsigned int j);
    void add_infected(unsigned int i);
    void remove_relation(unsigned int i, unsigned int j);

    const std::set<std::pair<unsigned int, unsigned int>>& relations() const;
    const std::set<unsigned int>* relations(unsigned int i) const;

    const std::set<unsigned int>& infected() const;
    const std::set<unsigned int>* infected(unsigned int i) const;


    float run(unsigned int virality, const std::set<std::pair<unsigned int, unsigned int>>& isolations) const;
    unsigned run_iteration(unsigned int virality);

  private:
    void update_infected(unsigned int i, unsigned int j);
    void update_infected(unsigned int i);

    // TODO: maybe use unordered_set or/and unordered_map
    const unsigned int size_;
    std::map<unsigned int, std::set<unsigned int>> relations_;
    std::map<unsigned int, std::set<unsigned int>> infected_relations_;
    std::set<std::pair<unsigned int, unsigned int>> all_relations_;
    std::set<unsigned int> all_infected_;
};

} /* namespace tp */

#endif /* INCLUDE_POPULATION_HPP */

#ifndef INCLUDE_NOTES_HPP
#define INCLUDE_NOTES_HPP

#include <filesystem>
#include <optional>
#include <vector>

namespace tp {

class notes {
  public:
    static std::optional<notes> from_file(const std::filesystem::path& path);
    const std::vector<unsigned int>& get_values() const;
    std::optional<unsigned int> operator[](unsigned int index) const;
    unsigned int size() const;
  private:
    std::vector<unsigned int> values_;
};

} /* namespace tp */

#endif /* INCLUDE_NOTES_HPP */

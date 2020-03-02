#include <magpie/core/configuration.hpp>

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <string>
#include <utility>

namespace mp
{
configuration::configuration (std::string filepath) : filepath_(std::move(filepath))
{
  if (!std::filesystem::exists(filepath_)) return;
  std::ifstream stream(filepath_);
  stream >> json_;
}
configuration::~configuration()
{
  std::ofstream stream(filepath_);
  stream << std::setw(2) << json_ << std::endl;
}
}

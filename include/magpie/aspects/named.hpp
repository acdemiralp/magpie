#ifndef MAGPIE_ASPECTS_NAMED_HPP
#define MAGPIE_ASPECTS_NAMED_HPP

#include <string>

#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT named
{
public:
  explicit named  (const std::string& name = "") : name_(name)
  {
    
  }
  named           (const named&  that) = default;
  named           (      named&& temp) = default;
  virtual ~named  ()                   = default;
  named& operator=(const named&  that) = default;
  named& operator=(      named&& temp) = default;

  const std::string& name    () const
  {
    return name_;
  }
  void               set_name(const std::string& name)
  {
    name_ = name;
  }

protected:
  std::string name_;
};
}

#endif
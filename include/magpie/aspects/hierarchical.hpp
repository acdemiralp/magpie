#ifndef MAGPIE_ASPECTS_HIERARCHICAL_HPP
#define MAGPIE_ASPECTS_HIERARCHICAL_HPP

#include <algorithm>
#include <vector>

#include <magpie/export.hpp>

namespace mp
{
template<typename derived>
class MAGPIE_EXPORT hierarchical
{
public:
  virtual ~hierarchical() = default;

  virtual void                 set_parent  (derived* parent)
  {
    if (parent_)
      parent_->children_.erase(std::remove(parent_->children_.begin(), parent_->children_.end(), static_cast<derived*>(this)), parent_->children_.end());
    
    parent_ = parent;

    if (parent_)
      parent_->children_.push_back(static_cast<derived*>(this));
  }
  void                         set_children(const std::vector<derived*>& children)
  {
    // This function breaks encapsulation as part of an informed design choice: Use knowingly and sparingly.
    children_ = children;
  }
  derived*                     parent      ()
  {
    return parent_  ;
  }
  const std::vector<derived*>& children    () const
  {
    return children_;
  }

protected:
  derived*              parent_  = nullptr;
  std::vector<derived*> children_;
};
}

#endif
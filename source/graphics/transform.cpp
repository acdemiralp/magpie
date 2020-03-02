#include <magpie/graphics/transform.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>

namespace mp
{
transform::transform()
{
  commit();
}

glm::vec3 transform::translation       (const bool absolute) const
{
  return absolute && parent_ ? parent_->translation(true) + translation_ : translation_;
}
glm::quat transform::rotation          (const bool absolute) const
{
  return absolute && parent_ ? parent_->rotation   (true) * rotation_    : rotation_;
}
glm::vec3 transform::rotation_euler    (const bool absolute) const
{
  return glm::degrees(glm::eulerAngles(rotation(absolute)));
}
glm::vec3 transform::scale             (const bool absolute) const
{
  return absolute && parent_ ? parent_->scale      (true) * scale_       : scale_;
}
glm::mat4 transform::matrix            (const bool absolute) const
{
  return absolute ? absolute_matrix_ : matrix_;
}
          
glm::vec3 transform::right             (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(1.0f, 0.0f, 0.0f);
}
glm::vec3 transform::up                (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(0.0f, 1.0f, 0.0f);
}
glm::vec3 transform::forward           (const bool absolute) const
{
  return rotation(absolute) * glm::vec3(0.0f, 0.0f, 1.0f);
}
          
void      transform::set_translation   (const glm::vec3& translation                , const bool absolute)
{
  translation_ = absolute && parent_ ? translation - parent_->translation(true) : translation;
  if (auto_commit_) commit();
}
void      transform::set_rotation      (const glm::quat& rotation                   , const bool absolute)
{
  rotation_ = absolute && parent_ ? rotation * glm::inverse(parent_->rotation(true)) : rotation;
  if (auto_commit_) commit();
}
void      transform::set_rotation_euler(const glm::vec3& rotation                   , const bool absolute)
{
  set_rotation(glm::quat(glm::radians(rotation)), absolute);
}
void      transform::set_scale         (const glm::vec3& scale                      , const bool absolute)
{
  scale_ = absolute && parent_ ? scale / parent_->scale(true) : scale;
  if (auto_commit_) commit();
}
void      transform::set_matrix        (const glm::mat4& matrix                     , const bool absolute)
{
  glm::vec3 skew       ;
  glm::vec4 perspective;
  matrix_ = absolute && parent_ ? matrix * glm::inverse(parent_->absolute_matrix_) : matrix;
  glm::decompose(matrix_, scale_, rotation_, translation_, skew, perspective);
  if (auto_commit_) commit();
}
          
void      transform::translate         (const glm::vec3& value                      , const bool absolute)
{
  set_translation(value + translation(absolute), absolute);
}
void      transform::rotate            (const glm::quat& value                      , const bool absolute, const bool postmultiply)
{
  set_rotation   (postmultiply ? rotation(absolute) * value : value * rotation(absolute), absolute);
}
void      transform::rotate_euler      (const glm::vec3& value                      , const bool absolute, const bool postmultiply)
{
  rotate         (glm::quat(glm::radians(value)), absolute, postmultiply);
}
void      transform::scale             (const glm::vec3& value                      , const bool absolute)
{
  set_scale      (value * scale      (absolute), absolute);
}
void      transform::look_at           (const glm::vec3& target, const glm::vec3& up, const bool absolute)
{
  set_rotation(glm::conjugate(glm::toQuat(glm::lookAt(translation(absolute), target, up))), absolute);
}
void      transform::reset             ()
{
  translation_ = glm::vec3(0.0f, 0.0f, 0.0f);
  rotation_    = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  scale_       = glm::vec3(1.0f, 1.0f, 1.0f);
  if (auto_commit_) commit();
}
          
void      transform::set_parent        (transform* parent)
{
  set_parent(parent, true);
}
void      transform::set_parent        (transform* parent, const bool maintain_absolute)
{
  hierarchical<transform>::set_parent(parent);
  if (maintain_absolute) set_matrix(absolute_matrix_, true);
  if (auto_commit_) commit();
}

bool      transform::auto_commit       () const
{
  return auto_commit_;
}
void      transform::set_auto_commit   (const bool auto_commit)
{
  auto_commit_ = auto_commit;
}
                                       
void      transform::commit            ()
{
  matrix_ = glm::translate(translation_) * glm::mat4_cast(rotation_) * glm::scale(scale_);
  update_hierarchy();
}

void      transform::update_hierarchy  ()
{
  absolute_matrix_ = parent_ ? parent_->absolute_matrix_ * matrix_ : matrix_;
  for (auto& child : children_) 
    child->update_hierarchy();
}
}

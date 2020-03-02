#ifndef MAGPIE_GRAPHICS_TRANSFORM_HPP
#define MAGPIE_GRAPHICS_TRANSFORM_HPP

#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <magpie/aspects/hierarchical.hpp>
#include <magpie/export.hpp>

namespace mp
{
class MAGPIE_EXPORT transform : public hierarchical<transform>
{
public:
  transform           ();
  transform           (const transform&  that) = default;
  transform           (      transform&& temp) = default;
  virtual ~transform  ()                       = default;
  transform& operator=(const transform&  that) = default;
  transform& operator=(      transform&& temp) = default;

  glm::vec3 translation       (const bool absolute = false) const;
  glm::quat rotation          (const bool absolute = false) const;
  glm::vec3 rotation_euler    (const bool absolute = false) const;
  glm::vec3 scale             (const bool absolute = false) const;
  glm::mat4 matrix            (const bool absolute = false) const;
                              
  glm::vec3 right             (const bool absolute = false) const;
  glm::vec3 up                (const bool absolute = false) const;
  glm::vec3 forward           (const bool absolute = false) const;
            
  void      set_translation   (const glm::vec3& translation, const bool absolute = false);
  void      set_rotation      (const glm::quat& rotation   , const bool absolute = false);
  void      set_rotation_euler(const glm::vec3& rotation   , const bool absolute = false);
  void      set_scale         (const glm::vec3& scale      , const bool absolute = false);
  void      set_matrix        (const glm::mat4& matrix     , const bool absolute = false);
                                                                           
  void      translate         (const glm::vec3& value      , const bool absolute = false);
  void      rotate            (const glm::quat& value      , const bool absolute = false, const bool post_multiply = false);
  void      rotate_euler      (const glm::vec3& value      , const bool absolute = false, const bool post_multiply = false);
  void      scale             (const glm::vec3& value      , const bool absolute = false);
  void      look_at           (const glm::vec3& target     , const glm::vec3& up = glm::vec3(0.0f, 1.0f, 0.0f), const bool absolute = false);
  void      reset             ();
            
  void      set_parent        (transform* parent) override;
  void      set_parent        (transform* parent, const bool maintain_absolute);
  
  bool      auto_commit       () const;
  void      set_auto_commit   (const bool auto_commit);

  void      commit            ();

protected:  
  void      update_hierarchy  ();

  glm::vec3 translation_     = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::quat rotation_        = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
  glm::vec3 scale_           = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::mat4 matrix_          ;
  glm::mat4 absolute_matrix_ ;
  bool      auto_commit_     = true;
};
}

#endif
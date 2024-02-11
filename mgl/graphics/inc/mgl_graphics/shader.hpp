#pragma once

#include "mgl_platform/api/program.hpp"
#include "mgl_platform/api/render_api.hpp"

#include "mgl_core/debug.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

#include "glm/glm.hpp"
namespace mgl::graphics
{
  class shader
  {
public:
    enum class uniform_type
    {
      BOOL,
      INT,
      UINT,
      FLOAT,
      VEC2,
      VEC3,
      VEC4,
      MAT2,
      MAT2X3,
      MAT2X4,
      MAT3,
      MAT3X2,
      MAT3X4,
      MAT4,
      MAT4X2,
      MAT4X3,
    };

    struct uniform_value
    {
      uniform_type type;
      union data
      {
        bool b;
        int32_t i;
        uint32_t u;
        float f;
        glm::vec2 vec2;
        glm::vec3 vec3;
        glm::vec4 vec4;
        glm::mat2 mat2;
        glm::mat2x3 mat2x3;
        glm::mat2x4 mat2x4;
        glm::mat3 mat3;
        glm::mat3x2 mat3x2;
        glm::mat3x4 mat3x4;
        glm::mat4 mat4;
        glm::mat4x2 mat4x2;
        glm::mat4x3 mat4x3;
      } data;

      uniform_value(bool value)
          : type(uniform_type::BOOL)
      {
        data.b = value;
      }

      uniform_value(int32_t value)
          : type(uniform_type::INT)
      {
        data.i = value;
      }

      uniform_value(uint32_t value)
          : type(uniform_type::UINT)
      {
        data.u = value;
      }

      uniform_value(float value)
          : type(uniform_type::FLOAT)
      {
        data.f = value;
      }

      uniform_value(const glm::vec2& value)
          : type(uniform_type::VEC2)
      {
        data.vec2 = value;
      }

      uniform_value(const glm::vec3& value)
          : type(uniform_type::VEC3)
      {
        data.vec3 = value;
      }

      uniform_value(const glm::vec4& value)
          : type(uniform_type::VEC4)
      {
        data.vec4 = value;
      }

      uniform_value(const glm::mat2& value)
          : type(uniform_type::MAT2)
      {
        data.mat2 = value;
      }

      uniform_value(const glm::mat2x3& value)
          : type(uniform_type::MAT2X3)
      {
        data.mat2x3 = value;
      }

      uniform_value(const glm::mat2x4& value)
          : type(uniform_type::MAT2X4)
      {
        data.mat2x4 = value;
      }

      uniform_value(const glm::mat3& value)
          : type(uniform_type::MAT3)
      {
        data.mat3 = value;
      }

      uniform_value(const glm::mat3x2& value)
          : type(uniform_type::MAT3X2)
      {
        data.mat3x2 = value;
      }

      uniform_value(const glm::mat3x4& value)
          : type(uniform_type::MAT3X4)
      {
        data.mat3x4 = value;
      }

      uniform_value(const glm::mat4& value)
          : type(uniform_type::MAT4)
      {
        data.mat4 = value;
      }

      uniform_value(const glm::mat4x2& value)
          : type(uniform_type::MAT4X2)
      {
        data.mat4x2 = value;
      }

      uniform_value(const glm::mat4x3& value)
          : type(uniform_type::MAT4X3)
      {
        data.mat4x3 = value;
      }
    };

    virtual ~shader() = default;

    virtual void prepare() = 0;
    virtual const mgl::string_list& attributes() = 0;
    virtual void load() = 0;

    mgl::platform::api::program_ref& api() { return m_program; }

    void bind()
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      mgl::platform::api::render_api::enable_program(m_program);
      mgl::platform::api::render_api::set_program_attributes(attributes());
    }

    void unbind()
    {
      MGL_CORE_ASSERT(false, "Not implemented");
      mgl::platform::api::render_api::disable_program();
    }

    void unload()
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->release();
    }

    void get_uniform_value(const std::string& name, bool& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, uint8_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, uint16_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, uint32_t& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, int& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, float& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::vec2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::vec3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::vec4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat2x3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat2x4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat3x2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat3x4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat4& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat4x2& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, glm::mat4x3& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value);
    }

    void get_uniform_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value, size);
    }

    void get_uniform_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value, size);
    }

    void get_uniform_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value, size);
    }

    void get_uniform_value(const std::string& name, int* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value, size);
    }

    void get_uniform_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->get_value(name, value, size);
    }

    void set_uniform_value(const std::string& name, bool value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint8_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint16_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint32_t value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, int value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, float value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::vec2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::vec3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::vec4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2x3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat2x4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3x2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat3x4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4x2 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const glm::mat4x3 value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint8_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint16_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, uint32_t* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, int* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, float* value, size_t size)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      m_program->set_value(name, value);
    }

    void set_uniform_value(const std::string& name, const uniform_value& value)
    {
      MGL_CORE_ASSERT(m_program != nullptr, "Program is null");
      switch(value.type)
      {
        case uniform_type::BOOL: m_program->set_value(name, value.data.b); break;
        case uniform_type::INT: m_program->set_value(name, value.data.i); break;
        case uniform_type::UINT: m_program->set_value(name, value.data.u); break;
        case uniform_type::FLOAT: m_program->set_value(name, value.data.f); break;
        case uniform_type::VEC2: m_program->set_value(name, value.data.vec2); break;
        case uniform_type::VEC3: m_program->set_value(name, value.data.vec3); break;
        case uniform_type::VEC4: m_program->set_value(name, value.data.vec4); break;
        case uniform_type::MAT2: m_program->set_value(name, value.data.mat2); break;
        case uniform_type::MAT2X3: m_program->set_value(name, value.data.mat2x3); break;
        case uniform_type::MAT2X4: m_program->set_value(name, value.data.mat2x4); break;
        case uniform_type::MAT3: m_program->set_value(name, value.data.mat3); break;
        case uniform_type::MAT3X2: m_program->set_value(name, value.data.mat3x2); break;
        case uniform_type::MAT3X4: m_program->set_value(name, value.data.mat3x4); break;
        case uniform_type::MAT4: m_program->set_value(name, value.data.mat4); break;
        case uniform_type::MAT4X2: m_program->set_value(name, value.data.mat4x2); break;
        case uniform_type::MAT4X3: m_program->set_value(name, value.data.mat4x3); break;
      }
    }

protected:
    mgl::platform::api::program_ref m_program = nullptr;
  };

  using shader_ref = mgl::ref<shader>;

} // namespace mgl::graphics

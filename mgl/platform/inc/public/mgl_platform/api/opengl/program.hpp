#pragma once

#include "mgl_platform/api/program.hpp"

#include "mgl_core/memory.hpp"

#include "mgl_opengl/program.hpp"

namespace mgl::platform::api::backends
{
  class ogl_program;
  using ogl_program_ref = mgl::ref<ogl_program>;

  class ogl_program : public mgl::platform::api::program
  {
public:
    ogl_program(const std::string& vs_source,
                const std::string& fs_source,
                const std::string& gs_source = "",
                const std::string& tes_source = "",
                const std::string& tcs_source = "",
                const std::string& filename = "");

    virtual ~ogl_program() = default;

    virtual void release() override final { m_program->release(); }

    virtual void bind() override final { m_program->bind(); }

    virtual void unbind() override final { m_program->unbind(); }


    virtual void get_value(const std::string& name, bool& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, uint8_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, uint16_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, uint32_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, uint64_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, int8_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, int16_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, int32_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, float& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, double& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::vec2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::vec3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::vec4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat2x3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat2x4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat3x2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat3x4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat4x2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, glm::mat4x3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::uint8_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::uint16_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::uint32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::int8_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::int16_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::int32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::float32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, mgl::float64_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, uint8_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, uint16_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, uint32_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, int8_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, int16_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, int32_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, int64_t& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value);
    }

    virtual void get_value(const std::string& name, float* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }

    virtual void get_value(const std::string& name, double* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->get_value(name, value, size);
    }    

    virtual void set_value(const std::string& name, bool value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, uint8_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, uint16_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, uint32_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, uint64_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, int8_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, int16_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, int32_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, int64_t value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, float value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, double value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::vec2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::vec3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::vec4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat2x3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat2x4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat3x2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat3x4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat4& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const glm::mat4x2& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }


    virtual void set_value(const std::string& name, const glm::mat4x3& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::uint8_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::uint16_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::uint32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::uint64_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::int8_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::int16_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::int32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::int64_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::float32_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }

    virtual void set_value(const std::string& name, const mgl::float64_buffer& value) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value);
    }



    virtual void set_value(const std::string& name, uint8_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, uint16_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, uint32_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, int8_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, int16_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, int32_t* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, float* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual void set_value(const std::string& name, double* value, size_t size) override final
    {
      if(!m_program->has_uniform(name))
      {
        return;
      }
      m_program->set_value(name, value, size);
    }

    virtual bool has_uniform(const std::string& name) override final
    {
      return m_program->has_uniform(name);
    }
    
    mgl::opengl::program_ref& native() { return m_program; }

protected:
    mgl::opengl::program_ref m_program;
  };

  using program_ref = mgl::ref<program>;
} // namespace mgl::platform::api::backends
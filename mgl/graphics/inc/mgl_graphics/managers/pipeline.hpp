#pragma once

#include "mgl_graphics/manager.hpp"
#include "mgl_graphics/pipeline.hpp"

namespace mgl::graphics
{
  class pipeline_manager : public manager<pipeline_ref>
  {
public:
    pipeline_manager() = default;
    ~pipeline_manager() = default;

    virtual void on_add(const pipeline_ref& p, const std::string& name) override final
    {
      p->load();
    }
    virtual void on_remove(const pipeline_ref& p, const std::string& name) override final
    {
      p->unload();
    }

    static pipeline_manager& instance()
    {
      static pipeline_manager instance;
      return instance;
    }
  };

  inline pipeline_manager& pipelines()
  {
    return pipeline_manager::instance();
  }
} // namespace mgl::graphics
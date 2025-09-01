#pragma once
#include "mgl_graphics/pipeline.hpp"

class custom_pipeline : public mgl::graphics::pipeline
{
  public:
  custom_pipeline() = default;

  virtual void prepare() override final;
  virtual void load() override final;
};

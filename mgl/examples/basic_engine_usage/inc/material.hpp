#pragma once
#include "mgl_engine/material.hpp"

class custom_material : public mgl::engine::material
{
  public:
  custom_material();

  virtual void prepare() override final;
  virtual mgl::window::program_ref& program() override final;
  virtual mgl::string_list& attributes() override final;
  virtual void release() override final;
};

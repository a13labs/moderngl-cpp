#include "mgl_core/log.hpp"
#include "mgl_registry/loaders/image.hpp"
#include "mgl_registry/loaders/shader.hpp"
#include "mgl_registry/loaders/text.hpp"
#include "mgl_registry/registry.hpp"
#include "gtest/gtest.h"

TEST(mgl_test_load_image, load_image)
{
  auto image = mgl::registry::load_image("test.png", mgl::registry::loader_options());
  EXPECT_NE(image, nullptr);
  EXPECT_EQ(image->get_type(), mgl::registry::resource::type::image);
  EXPECT_EQ(image->width(), 128);
  EXPECT_EQ(image->height(), 161);
  EXPECT_EQ(image->channels(), 3);
  EXPECT_EQ(image->data().size(), 128 * 161 * 3);
}

TEST(mgl_test_load_image, load_image_with_options)
{
  mgl::registry::loaders::image_loader_options options;
  options.flip_vertically = true;
  auto image = mgl::registry::load_image("test.png", options);
  EXPECT_NE(image, nullptr);
  EXPECT_EQ(image->get_type(), mgl::registry::resource::type::image);
  EXPECT_EQ(image->width(), 128);
  EXPECT_EQ(image->height(), 161);
  EXPECT_EQ(image->channels(), 3);
  EXPECT_EQ(image->data().size(), 128 * 161 * 3);
}

TEST(mgl_test_load_text, load_text)
{
  auto text = mgl::registry::load_text("test.txt", mgl::registry::loader_options());
  EXPECT_NE(text, nullptr);
  EXPECT_EQ(text->get_type(), mgl::registry::resource::type::text);
  EXPECT_EQ(text->data(), "Hello, World!");
}

TEST(mgl_test_load_text, load_text_with_options)
{
  mgl::registry::loaders::text_loader_options options;
  auto text = mgl::registry::load_text("test.txt", options);
  EXPECT_NE(text, nullptr);
  EXPECT_EQ(text->get_type(), mgl::registry::resource::type::text);
  EXPECT_EQ(text->data(), "Hello, World!");
}

TEST(mgl_test_loadt_shader, load_shader)
{
  auto shader = mgl::registry::load_shader("test.glsl", mgl::registry::loader_options());
  EXPECT_NE(shader, nullptr);
  EXPECT_EQ(shader->get_type(), mgl::registry::resource::type::shader);
}

TEST(mgl_test_loadt_shader, load_shader_with_options)
{
  mgl::registry::loaders::shader_loader_options options;
  options.type = mgl::registry::shader::type::FRAGMENT_SHADER;
  auto shader = mgl::registry::load_shader("test.fs", options);
  EXPECT_NE(shader, nullptr);
  EXPECT_EQ(shader->get_type(), mgl::registry::resource::type::shader);
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  mgl::log::init();
  mgl::registry::registry_ref registry = mgl::create_scope<mgl::registry::registry>();
  mgl::registry::register_dir(mgl::registry::resource::type::image, "file://data/images");
  mgl::registry::register_dir(mgl::registry::resource::type::text, "file://data/text");
  mgl::registry::register_dir(mgl::registry::resource::type::shader, "file://data/shaders");
  return RUN_ALL_TESTS();
}
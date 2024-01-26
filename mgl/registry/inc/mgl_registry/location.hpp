#pragma once

#include "mgl_core/io.hpp"
#include "mgl_core/memory.hpp"
#include "mgl_core/string.hpp"

namespace mgl::registry
{
  class location;
  class location_factory;
  struct url;
  using location_ref = mgl::ref<location>;
  using location_factory_ref = mgl::scope<location_factory>;

  struct url
  {
public:
    url() = default;
    ~url() = default;
    url(const std::string& url) { this->parse(url); }
    void parse(const std::string& url);

    std::string protocol, host, query;
    mgl::path path;
  };

  class location_factory
  {
public:
    virtual ~location_factory() = default;

    virtual bool can_handle(const url& url) const = 0;

    virtual location_ref factory(const url& url) const = 0;

    virtual const std::string& name() const = 0;
  };

  class location
  {
public:
    ~location() = default;

    bool is_null() const { return m_path.empty(); }

    const mgl::path& path() const { return m_path; }

    virtual istream_ref open(const std::string& path,
                             std::ios_base::openmode mode = std::ios_base::in) = 0;

    virtual bool operator==(const location& other) const = 0;

    virtual bool exists(const std::string& path) const = 0;

protected:
    location(const std::string& path)
        : m_path(mgl::path(path))
    { }

    location(const mgl::path& path)
        : m_path(path.string())
    { }

    location(const location& other)
        : m_path(other.m_path)
    { }

    void set_path(const mgl::path& path) { m_path = path; }

private:
    mgl::path m_path;
  };

  using locations = mgl::list<location_ref>;

  class null_location : public location, public location_factory
  {
public:
    null_location()
        : location(std::string())
    { }

    virtual istream_ref open(const std::string& path,
                             std::ios_base::openmode mode = std::ios_base::in) override final
    {
      return nullptr;
    }

    virtual bool operator==(const location& other) const override final { return false; }

    virtual bool exists(const std::string& path) const override final { return false; }

    virtual bool can_handle(const url& url) const override final { return false; }

    virtual location_ref factory(const url& url) const override final { return nullptr; }

    virtual const std::string& name() const override final;
  };

} // namespace mgl::registry
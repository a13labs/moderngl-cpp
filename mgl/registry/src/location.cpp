#include "mgl_registry/location.hpp"

namespace mgl::registry
{
  static const std::string null_location_name = "null";

  void url::parse(const std::string& url)
  {
    const std::string::size_type protocol_end = url.find("://");
    if(protocol_end == std::string::npos)
      return;

    protocol = url.substr(0, protocol_end);

    if(protocol == "file")
    {
      path = url.substr(protocol_end + 3);
      return;
    }

    std::string::size_type host_end = url.find('/', protocol_end + 3);
    if(host_end == std::string::npos)
    {
      host_end = url.length();
    }

    host = url.substr(protocol_end + 3, host_end - (protocol_end + 3));

    if(host_end == url.length())
      return;

    std::string::size_type query_begin = url.find('?');

    if(query_begin != std::string::npos)
    {
      query = url.substr(query_begin + 1);
      path = url.substr(host_end, query_begin - host_end);
    }
    else
    {
      path = url.substr(host_end);
    }
  }

} // namespace mgl::registry
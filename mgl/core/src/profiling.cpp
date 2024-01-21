#include "mgl_core/profiling.hpp"
#if MGL_PROFILING
#  include "mgl_core/debug.hpp"
#  include "mgl_core/log.hpp"
#  include "mgl_core/string.hpp"
#  include "version.hpp"

#  define JSON_HAS_RANGES 0 // FIXME: this is a workaround for a bug in nlohmann/json.hpp with CLANG
#  include <nlohmann/json.hpp>

namespace mgl::profiling
{
  static nlohmann::json s_trace_events;

  void instrumentor::begin_session(const std::string& name, const std::string& filepath)
  {
    std::lock_guard lock(m_mutex);
    m_file_path = filepath;
    if(m_current_session)
    {
      // If there is already a current session, then close it before beginning new one.
      // Subsequent profiling output meant for the original session will end up in the
      // newly opened session instead.  That's better than having badly formatted
      // profiling output.
      if(mgl::log::is_initialized()) // Edge case: begin_session() might be before Log::Init()
      {
        MGL_CORE_ERROR("instrumentor::begin_session('{0}') when session '{1}' already open.",
                       name,
                       m_current_session->name);
      }
      internal_end_session();
    }
    m_current_session = new instrumentation_session({ name });
  }

  void instrumentor::end_session()
  {
    std::lock_guard lock(m_mutex);
    internal_end_session();
  }

  void instrumentor::write_profile(const profile_result& result)
  {
    std::stringstream tidss;
    tidss << result.thread_id;
    uint64_t tid = std::stoull(tidss.str());

    nlohmann::json trace_event;
    trace_event["cat"] = "function," + result.category;
    trace_event["dur"] = result.elapsed_time.count();
    trace_event["name"] = result.name;
    trace_event["ph"] = "X";
    trace_event["pid"] = result.process_id;
    trace_event["tid"] = tid;
    trace_event["ts"] = result.start.count();

    std::lock_guard lock(m_mutex);
    s_trace_events.push_back(trace_event);
  }

  void instrumentor::write_trace_document()
  {
    nlohmann::json document;

    document["otherData"] = "{ \"version\":\"MGL v" MGL_SEM_VERSION "\"}"_json;
    document["traceEvents"] = s_trace_events;

    std::ofstream o(m_file_path);
    o << std::setw(4) << document << std::endl;
    o.flush();
  }

  // Note: you must already own lock on m_mutex before
  // calling internal_end_session()
  void instrumentor::internal_end_session()
  {
    if(m_current_session)
    {
      write_trace_document();
      delete m_current_session;
      m_current_session = nullptr;
    }
  }

  instrumentation_timer::instrumentation_timer(const char* name, const char* category)
      : m_name(name)
      , m_category(category)
      , m_stopped(false)
  {
    mStartTimepoint = std::chrono::steady_clock::now();
  }

  instrumentation_timer::~instrumentation_timer()
  {
    if(!m_stopped)
      stop();
  }

  void instrumentation_timer::stop()
  {
    auto end_timepoint = std::chrono::steady_clock::now();
    auto high_res_start = floating_point_microseconds{ mStartTimepoint.time_since_epoch() };
    auto elapsed_time =
        std::chrono::time_point_cast<std::chrono::microseconds>(end_timepoint).time_since_epoch() -
        std::chrono::time_point_cast<std::chrono::microseconds>(mStartTimepoint).time_since_epoch();

    instrumentor::get().write_profile({ m_name,
                                        high_res_start,
                                        elapsed_time,
                                        std::this_thread::get_id(),
                                        GET_PROCESS_ID(),
                                        m_category });

    m_stopped = true;
  }

} // namespace mgl::profiling
#endif
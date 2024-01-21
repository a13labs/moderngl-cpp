/*
   Copyright 2020 Alexandre Pires (c.alexandre.pires@gmail.com)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once
#ifndef MGL_PROFILING
#  define MGL_PROFILING 0
#endif
#if MGL_PROFILING
#  include "platform.hpp"
#  include "string.hpp"
#  include <chrono>
#  include <fstream>
#  include <sys/types.h>

#  include <thread>
#  ifdef MGL_PLATFORM_WINDOWS
#    include <process.h>
#    define GET_PROCESS_ID _getpid
#  else
#    include <unistd.h>
#    define GET_PROCESS_ID getpid
#  endif

namespace mgl::profiling
{
  using floating_point_microseconds = std::chrono::duration<double, std::micro>;

  struct profile_result
  {
    std::string name;

    floating_point_microseconds start;
    std::chrono::microseconds elapsed_time;
    std::thread::id thread_id;
    int process_id;
    std::string category;
  };

  struct instrumentation_session
  {
    std::string name;
  };

  class instrumentor
  {
private:
    std::mutex m_mutex;
    instrumentation_session* m_current_session;
    std::string m_file_path;

public:
    instrumentor()
        : m_current_session(nullptr)
    { }

    void begin_session(const std::string& name, const std::string& filepath = "results.json");

    void end_session();

    void write_profile(const profile_result& result);

    static instrumentor& get()
    {
      static instrumentor instance;
      return instance;
    }

private:
    void write_trace_document();

    // Note: you must already own lock on m_mutex before
    // calling internal_end_session()
    void internal_end_session();
  };

  class instrumentation_timer
  {
public:
    instrumentation_timer(const char* name, const char* category);

    ~instrumentation_timer();

    void stop();

private:
    const char *m_name, *m_category;
    std::chrono::time_point<std::chrono::steady_clock> mStartTimepoint;
    bool m_stopped;
  };

  namespace instrumentor_utils
  {

    template <size_t N>
    struct ChangeResult
    {
      char Data[N];
    };

    template <size_t N, size_t K>
    constexpr auto cleanup_output_string(const char (&expr)[N], const char (&remove)[K])
    {
      ChangeResult<N> result = {};

      size_t srcIndex = 0;
      size_t dstIndex = 0;
      while(srcIndex < N)
      {
        size_t matchIndex = 0;
        while(matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
              expr[srcIndex + matchIndex] == remove[matchIndex])
          matchIndex++;
        if(matchIndex == K - 1)
          srcIndex += matchIndex;
        result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
        srcIndex++;
      }
      return result;
    }
  } // namespace instrumentor_utils
} // namespace mgl::profiling

// Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#  if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) ||                      \
      (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#    define MGL_CORE_FUNC_SIG __PRETTY_FUNCTION__
#  elif defined(__DMC__) && (__DMC__ >= 0x810)
#    define MGL_CORE_FUNC_SIG __PRETTY_FUNCTION__
#  elif(defined(__FUNCSIG__) || (_MSC_VER))
#    define MGL_CORE_FUNC_SIG __FUNCSIG__
#  elif(defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) ||                                 \
      (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#    define MGL_CORE_FUNC_SIG __FUNCTION__
#  elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#    define MGL_CORE_FUNC_SIG __FUNC__
#  elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#    define MGL_CORE_FUNC_SIG __func__
#  elif defined(__cplusplus) && (__cplusplus >= 201103)
#    define MGL_CORE_FUNC_SIG __func__
#  else
#    define MGL_CORE_FUNC_SIG "MGL_CORE_FUNC_SIG unknown!"
#  endif

#  define MGL_PROFILE_BEGIN_SESSION() ::mgl::profiling::instrumentor::get().begin_session("MGL")
#  define MGL_PROFILE_END_SESSION() ::mgl::profiling::instrumentor::get().end_session()
#  define MGL_PROFILE_SCOPE(name, category)                                                        \
    constexpr auto fixedName =                                                                     \
        ::mgl::profiling::instrumentor_utils::cleanup_output_string(name, "__cdecl ");             \
    ::mgl::profiling::instrumentation_timer timer##__LINE__(fixedName.Data, category)
#  define MGL_PROFILE_FUNCTION(category) MGL_PROFILE_SCOPE(MGL_CORE_FUNC_SIG, category)
#else
#  define MGL_PROFILE_BEGIN_SESSION()
#  define MGL_PROFILE_END_SESSION()
#  define MGL_PROFILE_SCOPE(name, category)
#  define MGL_PROFILE_FUNCTION(category)
#endif
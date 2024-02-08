/**
 * @file profiling.hpp
 * @brief This file contains the definitions and declarations related to profiling and instrumentation.
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
  /**
   * @brief Alias for a floating-point duration in microseconds.
   */
  using floating_point_microseconds = std::chrono::duration<double, std::micro>;

  /**
   * @brief Structure representing the result of a profiling session.
   */
  struct profile_result
  {
    std::string name; ///< The name of the profiled section.
    floating_point_microseconds start; ///< The start time of the profiled section.
    std::chrono::microseconds elapsed_time; ///< The elapsed time of the profiled section.
    std::thread::id thread_id; ///< The ID of the thread executing the profiled section.
    int process_id; ///< The ID of the process executing the profiled section.
    std::string category; ///< The category of the profiled section.
  };

  /**
   * @brief Structure representing an instrumentation session.
   */
  struct instrumentation_session
  {
    std::string name; ///< The name of the instrumentation session.
  };

  /**
   * @brief Class for instrumenting code and collecting profiling data.
   */
  class instrumentor
  {
private:
    std::mutex m_mutex; ///< Mutex for thread-safe access to the instrumentor.
    instrumentation_session* m_current_session; ///< Pointer to the current instrumentation session.
    std::string m_file_path; ///< The file path to write the profiling results.

public:
    /**
     * @brief Default constructor for the instrumentor class.
     */
    instrumentor()
        : m_current_session(nullptr)
    { }

    /**
     * @brief Begins a new instrumentation session.
     * @param name The name of the instrumentation session.
     * @param filepath The file path to write the profiling results. Default is "results.json".
     */
    void begin_session(const std::string& name, const std::string& filepath = "results.json");

    /**
     * @brief Ends the current instrumentation session.
     */
    void end_session();

    /**
     * @brief Writes a profile result to the profiling results file.
     * @param result The profile result to write.
     */
    void write_profile(const profile_result& result);

    /**
     * @brief Gets the singleton instance of the instrumentor class.
     * @return The instrumentor instance.
     */
    static instrumentor& get()
    {
      static instrumentor instance;
      return instance;
    }

private:
    /**
     * @brief Writes the trace document to the profiling results file.
     */
    void write_trace_document();

    /**
     * @brief Ends the current instrumentation session internally.
     * @note This function assumes that the caller already owns a lock on m_mutex.
     */
    void internal_end_session();
  };

  /**
   * @brief Class for measuring the execution time of a code block.
   */
  class instrumentation_timer
  {
public:
    /**
     * @brief Constructs an instrumentation timer with the specified name and category.
     * @param name The name of the timer.
     * @param category The category of the timer.
     */
    instrumentation_timer(const char* name, const char* category);

    /**
     * @brief Destructor for the instrumentation timer.
     */
    ~instrumentation_timer();

    /**
     * @brief Stops the instrumentation timer.
     */
    void stop();

private:
    const char* m_name; ///< The name of the timer.
    const char* m_category; ///< The category of the timer.
    std::chrono::time_point<std::chrono::steady_clock>
        mStartTimepoint; ///< The start time point of the timer.
    bool m_stopped; ///< Flag indicating whether the timer has been stopped.
  };

  namespace instrumentor_utils
  {
    /**
     * @brief Helper struct for changing the result type of a function.
     * @tparam N The size of the result data.
     */
    template <size_t N>
    struct ChangeResult
    {
      char Data[N]; ///< The result data.
    };

    /**
     * @brief Cleans up the output string by removing a specified substring.
     * @tparam N The size of the expression string.
     * @tparam K The size of the remove string.
     * @param expr The expression string.
     * @param remove The substring to remove.
     * @return The cleaned up output string.
     */
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

/**
 * @brief Macro for beginning a profiling session.
 */
#  define MGL_PROFILE_BEGIN_SESSION() ::mgl::profiling::instrumentor::get().begin_session("MGL")

/**
 * @brief Macro for ending the current profiling session.
 */
#  define MGL_PROFILE_END_SESSION() ::mgl::profiling::instrumentor::get().end_session()

/**
 * @brief Macro for profiling a code block with a specified name and category.
 * @param name The name of the code block.
 * @param category The category of the code block.
 */
#  define MGL_PROFILE_SCOPE(name, category)                                                        \
    constexpr auto fixedName =                                                                     \
        ::mgl::profiling::instrumentor_utils::cleanup_output_string(name, "__cdecl ");             \
    ::mgl::profiling::instrumentation_timer timer##__LINE__(fixedName.Data, category)

/**
 * @brief Macro for profiling a function with a specified category.
 * @param category The category of the function.
 */
#  define MGL_PROFILE_FUNCTION(category) MGL_PROFILE_SCOPE(MGL_CORE_FUNC_SIG, category)
#else
#  define MGL_PROFILE_BEGIN_SESSION()
#  define MGL_PROFILE_END_SESSION()
#  define MGL_PROFILE_SCOPE(name, category)
#  define MGL_PROFILE_FUNCTION(category)
#endif
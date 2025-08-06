/**
 * @file timer.hpp
 * @brief Contains the definition of the Timer class.
*/
#pragma once

#include <chrono>

/**
 * \brief The mgl namespace encapsulates the core functionalities of the MGL library, providing a structured and organized way to access these utilities.
 * It includes sub-namespaces such as io (input/output), log (logging) and profiling (profiling and instrumentation). 
 * These namespaces provide specific functionalities for managing file operations, logging messages at different levels, and measuring the execution time of code blocks or functions respectively.
 */
namespace mgl
{

/**
 * \brief A type alias representing a point in time using the system clock from the C++ standard library's chrono module.
 */
  using timepoint = std::chrono::time_point<std::chrono::system_clock>;

  /**
   * @brief A class that provides timing functionality.
   */
  class Timer
  {
/**
 * \brief The FrameTime struct represents a time frame with start and end points, providing methods to get the duration of that frame in seconds.
 */
    struct FrameTime
    {
      float current; /**< The current time in seconds. */
      float
          delta; /**< The time difference between the current frame and the previous frame in seconds. */
    };

public:
    /**
     * @brief Default constructor.
     */
    Timer();

    /**
     * @brief Destructor.
     */
    ~Timer() = default;

    /**
     * @brief Checks if the timer is paused.
     * @return True if the timer is paused, false otherwise.
     */
    bool is_paused();

    /**
     * @brief Checks if the timer is running.
     * @return True if the timer is running, false otherwise.
     */
    bool is_running();

    /**
     * @brief Gets the elapsed time since the timer started.
     * @return The elapsed time in seconds.
     */
    float time();

    /**
     * @brief Advances the timer to the next frame.
     * @return The frame time information.
     */
    Timer::FrameTime next_frame();

    /**
     * @brief Starts the timer.
     */
    void start();

    /**
     * @brief Pauses the timer.
     */
    void pause();

    /**
     * @brief Stops the timer and returns the total elapsed time.
     * @return The total elapsed time in seconds.
     */
    float stop();

    /**
     * @brief Toggles the pause state of the timer.
     * If the timer is paused, it will be resumed. If the timer is running, it will be paused.
     */
    void toogle_pause();

private:
    timepoint m_start_time; /**< The time point when the timer started. */
    timepoint m_stop_time; /**< The time point when the timer stopped. */
    timepoint m_pause_time; /**< The time point when the timer was paused. */
    float m_last_frame; /**< The time of the last frame in seconds. */
    std::chrono::nanoseconds m_offset; /**< The time offset when the timer was paused. */
    bool m_paused; /**< Flag indicating if the timer is paused. */
    bool m_running; /**< Flag indicating if the timer is running. */
  };

  inline bool Timer::is_paused()
  {
    return m_paused;
  }

  inline bool Timer::is_running()
  {
    return m_running;
  }

  inline void Timer::toogle_pause()
  {
    if(m_paused)
    {
      start();
    }
    else
    {
      pause();
    }
  }

} // namespace  mgl
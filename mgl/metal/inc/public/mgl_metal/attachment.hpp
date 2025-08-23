#pragma once

#include "mgl_core/memory.hpp"
#include <cstdint>

namespace mgl::metal
{
  class context;
  using context_ref = mgl::ref<context>;

  class attachment;
  using attachment_ref = mgl::ref<attachment>;
  using attachments_ref = mgl::ref_list<attachment>;

  /**
   * @brief The attachment class represents an attachment in the Metal context.
   *
   * It provides information about the attachment's type, dimensions, components, samples, depth,
   * and the associated context.
   */
  class attachment
  {
public:
    /**
     * @brief The type enum represents the type of the attachment.
     *
     * The type can be either a texture or a renderbuffer.
     */
    enum type
    {
      TEXTURE,
      RENDERBUFFER
    };

    virtual ~attachment() = default;

    /**
     * @brief The attachment_type method returns the type of the attachment.
     *
     * @return The type of the attachment.
     */
    virtual attachment::type attachment_type() = 0;

    /**
     * @brief The width method returns the width of the attachment.
     *
     * @return The width of the attachment.
     */
    virtual int32_t width() const = 0;

    /**
     * @brief The height method returns the height of the attachment.
     *
     * @return The height of the attachment.
     */
    virtual int32_t height() const = 0;

    /**
     * @brief The samples method returns the number of samples of the attachment.
     *
     * @return The number of samples of the attachment.
     */
    virtual int32_t samples() const = 0;

    /**
     * @brief The components method returns the number of components of the attachment.
     *
     * @return The number of components of the attachment.
     */
    virtual int32_t components() const = 0;

    /**
     * @brief The depth method returns whether the attachment is a depth attachment.
     *
     * @return True if the attachment is a depth attachment, false otherwise.
     */
    virtual bool depth() const = 0;

    /**
     * @brief The ctx method returns the context associated with the attachment.
     *
     * @return The context associated with the attachment.
     */
    virtual const context_ref& ctx() const = 0;
  };

} // namespace mgl::metal

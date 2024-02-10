#pragma once

#include "mgl_core/memory.hpp"
namespace mgl::opengl
{
  class context;
  using context_ref = mgl::ref<context>;

  class attachment;
  using attachment_ref = mgl::ref<attachment>;
  using attachments_ref = mgl::ref_list<attachment>;

  /**
   * @brief The attachment class represents an attachment in the OpenGL context.
   * 
   * It provides information about the attachment's type, dimensions, components, samples, depth, OpenGL object ID, and the associated context.
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

    /**
     * @brief The attachment_type method returns the type of the attachment.
     * 
     * @return The type of the attachment.
     */
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
     * @brief The components method returns the number of components of the attachment.
     * 
     * @return The number of components of the attachment.
     */
    virtual int32_t components() const = 0;

    /**
     * @brief The samples method returns the number of samples of the attachment.
     * 
     * @return The number of samples of the attachment.
     */
    virtual int32_t samples() const = 0;

    /**
     * @brief The depth method returns whether the attachment is a depth attachment.
     * 
     * @return Whether the attachment is a depth attachment.
     */
    virtual bool depth() const = 0;

    /**
     * @brief The glo method returns the OpenGL object ID of the attachment.
     * 
     * @return The OpenGL object ID of the attachment.
     */
    virtual int32_t glo() const = 0;

    /**
     * @brief The ctx method returns the associated context of the attachment.
     * 
     * @return The associated context of the attachment.
     */
    virtual const context_ref& ctx() const = 0;
  };

} // namespace  mgl::opengl
# mgl_core
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_core::mgl_core
)

# mgl_opengl
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_opengl::mgl_opengl
)

# mgl_window
target_link_libraries(
    ${MGL_CURRENT_TARGET}
    PRIVATE
    mgl_window::mgl_window
)

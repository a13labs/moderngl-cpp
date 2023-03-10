# Set project configuration here

set(MGL_NAME "CPP Application")
set(MGL_DESCRIPTION "CPP Application Template" )
set(MGL_VERSION_MAJOR 0)
set(MGL_VERSION_MINOR 0)
set(MGL_VERSION_RELEASE 1)
set(MGL_SEMANTIC_VERSION "${MGL_VERSION_MAJOR}.${MGL_VERSION_MINOR}.${MGL_VERSION_RELEASE}")
set(MGL_HOMEPAGE_URL "https://example.org")
set(MGL_SHORT "app")
set(MGL_LANG_STD cxx_std_17)
set(MGL_BUILD_TESTS ON)

# Add external packages, expacted value 3 values per entry: Package_Name GIT_REPO GIT_TAG
set(
    MGL_EXTERNAL_PROJECTS 
    glm     "https://github.com/g-truc/glm"                                      0.9.9.8
    spdlog  "https://github.com/gabime/spdlog.git"                               v1.10.0   
    json    "https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent" v3.11.2 
)

set(MGL_LIBRARIES "mgl_core" "mgl_opengl" "mgl_window")

set(MGL_BUILD_EXAMPLES OFF)
set(
    MGL_EXAMPLES_EXTERNAL_PROJECTS 
    NumCpp     "https://github.com/dpilger26/NumCpp.git"                        Version_2.8.0
)
set(MGL_EXAMPLES 
    "basic_simple_color_triangle"
    "basic_alpha_blending"
    "compute_shader"
    "basic_index_buffer"
    "basic_perspective_projection"
    "basic_uniforms_and_attributes"
   )
set(NUMCPP_NO_USE_BOOST ON)
set(MGL_SHOW_CMAKE_VARS OFF)
set(MGL_BUILD_TESTS OFF)
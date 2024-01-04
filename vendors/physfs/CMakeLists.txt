option(PHYSFS_ARCHIVE_ZIP "Enable ZIP support" ON)
option(PHYSFS_ARCHIVE_7Z "Enable 7zip support" ON)
option(PHYSFS_ARCHIVE_GRP "Enable Build Engine GRP support" OFF)
option(PHYSFS_ARCHIVE_WAD "Enable Doom WAD support" OFF)
option(PHYSFS_ARCHIVE_HOG "Enable Descent I/II HOG support" OFF)
option(PHYSFS_ARCHIVE_MVL "Enable Descent I/II MVL support" OFF)
option(PHYSFS_ARCHIVE_QPAK "Enable Quake I/II QPAK support" OFF)
option(PHYSFS_ARCHIVE_SLB "Enable I-War / Independence War SLB support" OFF)
option(PHYSFS_ARCHIVE_ISO9660 "Enable ISO9660 support" OFF)
option(PHYSFS_ARCHIVE_VDF "Enable Gothic I/II VDF archive support" OFF)
option(PHYSFS_BUILD_TEST "Build stdio test program." OFF)
option(PHYSFS_DISABLE_INSTALL "Disable installing PhysFS" ON)
option(PHYSFS_BUILD_DOCS "Build doxygen based documentation" OFF)

# Set the path where you want to download Google Test
set(PHYSFS_INSTALL_DIR "${CMAKE_BINARY_DIR}/physfs" CACHE PATH "Path to physfs installation directory")

# Use FetchContent to download physfs
FetchContent_Declare(
    physfs
    GIT_REPOSITORY https://github.com/icculus/physfs.git
    GIT_TAG        release-3.2.0
)

# FetchContent will download physfs and build it at configure time
FetchContent_MakeAvailable(physfs)

set(PHYSFS_BINARY_DIR "${physfs_BINARY_DIR}" CACHE PATH "Path to physfs binary directory")
set(PHYSFS_INCLUDE_DIR "${PHYSFS_SOURCE_DIR}/src" CACHE PATH "Path to physfs include directory")
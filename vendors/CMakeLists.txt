# physfs
option(PHYSFS_ARCHIVE_7Z OFF)
option(PHYSFS_ARCHIVE_GRP OFF)
option(PHYSFS_ARCHIVE_WAD OFF)
option(PHYSFS_ARCHIVE_HOG OFF)
option(PHYSFS_ARCHIVE_MVL OFF)
option(PHYSFS_ARCHIVE_QPAK OFF)
option(PHYSFS_ARCHIVE_SLB OFF)
option(PHYSFS_ARCHIVE_ISO9660 OFF)
option(PHYSFS_ARCHIVE_VDF OFF)
option(PHYSFS_ARCHIVE_VDF OFF)
option(PHYSFS_BUILD_SHARED OFF)
option(PHYSFS_BUILD_TEST OFF)
option(PHYSFS_BUILD_DOCS OFF)
add_subdirectory(physfs)

# mojoAL
add_subdirectory(mojoAL)

# spdlog
add_subdirectory(spdlog)

# glm
add_subdirectory(glm)

# glad
add_subdirectory(glad)

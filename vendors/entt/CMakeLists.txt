# Use FetchContent to download entt
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG        v3.12.2 
)

# FetchContent will download entt and build it at configure time
FetchContent_MakeAvailable(entt)

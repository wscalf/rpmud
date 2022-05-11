target_sources(objects
    PUBLIC
        CommandSystem.cpp
        Command.cpp
        Segment.cpp
    )

add_subdirectory(tests)
add_subdirectory(builtin)

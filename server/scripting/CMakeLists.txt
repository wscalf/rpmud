target_sources(objects
    PUBLIC
        CommandSystem.cpp
        Command.cpp
    )

add_subdirectory(tests)
add_subdirectory(builtin)

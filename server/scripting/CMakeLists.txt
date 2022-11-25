target_sources(objects
    PUBLIC
        ScriptSystem.cpp
        ScriptObject.cpp
        CommandSystem.cpp
        Command.cpp
        Segment.cpp
        Variant.cpp
    )

add_subdirectory(duktape)
add_subdirectory(tests)
add_subdirectory(builtin)

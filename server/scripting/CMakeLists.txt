target_sources(objects
    PUBLIC
        ScriptCommand.h
        ScriptCommand.cpp
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

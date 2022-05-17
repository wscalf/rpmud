add_subdirectory(tests)

target_sources(objects
    PUBLIC
        DirectTransition.cpp
        Transition.cpp
        MUDObject.cpp
        Room.cpp
        LoginProcess.cpp
        Player.cpp
        World.cpp
    )
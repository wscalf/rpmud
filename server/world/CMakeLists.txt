add_subdirectory(tests)

target_sources(objects
    PUBLIC
        Constants.cpp
        DirectTransition.cpp
        Transition.cpp
        MUDObject.cpp
        Room.cpp
        LoginProcess.cpp
        Player.cpp
        World.cpp
    )
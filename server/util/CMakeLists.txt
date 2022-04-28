add_subdirectory(tests)

target_sources(objects
    PUBLIC
        ThreadPool.cpp
        UUID.cpp
        Log.cpp
    )
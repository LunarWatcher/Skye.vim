set (SOURCE_FILES 
    # Internals
    skye/QueryEngine.cpp
    skye/GitHubConnector.cpp
    # Exposing
    skye/DynamicExposure.cpp
    )

add_library(skye SHARED ${SOURCE_FILES})

# Add conan includes
target_include_directories(skye PUBLIC ${CONAN_INCLUDE_DIRS})
# Add this source directory
target_include_directories(skye PUBLIC ./)

# Link conan libraries
target_link_libraries(skye ${CONAN_LIBS})

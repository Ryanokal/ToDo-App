cmake_minimum_required(VERSION 3.10)
project(App)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base)

if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found")
endif()

# Find SQLite3
find_package(SQLite3 REQUIRED)

if(NOT SQLite3_FOUND)
    message(FATAL_ERROR "SQLite3 not found")
endif()

# Define the source files
set(SOURCES
        src/main.cpp
        src/MyApp.cpp
        src/MyApp.h
        src/MyFrame.cpp
        src/MyFrame.h
        src/TaskDatabase.cpp
        src/TaskDatabase.h
)

# Include directories
include_directories(${wxWidgets_INCLUDE_DIRS} ${SQLite3_INCLUDE_DIRS} src)

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link wxWidgets and SQLite3 libraries
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${SQLite3_LIBRARIES})

# Platform-specific settings
if(WIN32)
    # Windows-specific settings (e.g., set subsystem)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE YES)
elseif(APPLE)
    # macOS-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif(UNIX)
    # Linux-specific settings
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

cmake_minimum_required(VERSION 3.10)
project(Min-DevKit)


string(REGEX REPLACE "(.*)/" "" THIS_PACKAGE_NAME "${CMAKE_CURRENT_SOURCE_DIR}")


if (${CMAKE_GENERATOR} MATCHES "Xcode")
         if (${XCODE_VERSION} VERSION_LESS 10)
              message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
            return ()
        endif ()
endif ()



# Add the Lib, if it exists
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/c74support/max-includes/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/c74support/max-includes)
endif ()


# Generate a project for every folder in the "projects" folder
SUBDIRLIST(PROJECT_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/projects)
foreach (project_dir ${PROJECT_DIRS})
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/projects/${project_dir}/CMakeLists.txt")
        message("Generating: ${project_dir}")
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/projects/${project_dir})
    endif ()
endforeach ()
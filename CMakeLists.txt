cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(rodos_plugin CXX C)
set(CMAKE_CXX_STANDARD 14)


# APPEND Findrodos.cmake address
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Find Rodos
find_package(rodos REQUIRED)

# Find Gazebo
find_package(gazebo REQUIRED)



###############   With RODOS
#
#${MY_HW_SRCS} ${MY_SRCS} ${RODOS_SUPPORT_LIBS}

include_directories(${GAZEBO_INCLUDE_DIRS} ${RODOS_INCLUDE_DIRS} )
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")


# Build our plugins
add_library(rodos_plugin SHARED rodos_plugin.cc)
target_link_libraries(rodos_plugin ${GAZEBO_LIBRARIES})

add_executable(vel vel.cc)
if (${gazebo_VERSION_MAJOR} LESS 6)
    # These two
    include(FindBoost)
    find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex)
    target_link_libraries(vel ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES} ${RODOS_LIBRARIES})
else ()
    target_link_libraries(vel ${GAZEBO_LIBRARIES} ${RODOS_LIBRARIES})
endif ()
include_directories(~/home/alejandro/rodos-workspace/rodos/api)
include_directories(~/home/alejandro/rodos-workspace/rodos)

##################################################################33


###############   Without RODOS

#${MY_HW_SRCS} ${MY_SRCS} ${RODOS_SUPPORT_LIBS}
#include_directories(${GAZEBO_INCLUDE_DIRS} )
#link_directories(${GAZEBO_LIBRARY_DIRS} )
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS} ")
#
#
## Build our plugins
##Build Rodos_plugin
#add_library(rodos_plugin SHARED rodos_plugin.cc)
#target_link_libraries(rodos_plugin ${GAZEBO_LIBRARIES})
#
#
## Add standalone Executable
#add_executable(vel vel.cc)
#if (${gazebo_VERSION_MAJOR} LESS 6)
#    # These two
#    include(FindBoost)
#    find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex)
#    target_link_libraries(vel ${GAZEBO_LIBRARIES} ${Boost_LIBRARIES})
#else ()
#    target_link_libraries(vel ${GAZEBO_LIBRARIES} )
#endif ()


###########################################################################

#${CPP_COMP}   ${CFLAGS} ${CPPFLAGS} ${INCLUDES} ${FIRST_LINK_OBJ} -o tst  $* ${LINKFLAGS}

add_custom_command(TARGET rodos_plugin POST_BUILD COMMAND
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/world/Glider $ENV{HOME}/.gazebo/models/Glider)


#extra information No relevant code
# -D CMAKE_FIND_DEBUG_MODE=ON
#CMAKE_PREFIX_PATH=/home/alejandro/rodos-workspace
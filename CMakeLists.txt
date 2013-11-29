project(geser)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS -std=c++11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBXML++ REQUIRED libxml++-2.6)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)

if(RSVG_VERSION VERSION_LESS 2.36.3)
    set(RSVG_CAIRO_INCLUDE ON)
endif()

include_directories(${PROJECT_SOURCE_DIR} ${LIBXML++_INCLUDE_DIRS} ${RSVG_INCLUDE_DIRS} ${GTKMM3_INCLUDE_DIRS})

file(GLOB SOURCE ${PROJECT_NAME}/*.cpp ${PROJECT_NAME}/private/*.cpp)
file(GLOB HEADER ${PROJECT_NAME}/*.hpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE})
target_link_libraries(${PROJECT_NAME}	${LIBXML++_LIBRARIES}
					${RSVG_LIBRARIES}
					${GTKMM3_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${HEADER}")

install(TARGETS ${PROJECT_NAME} 
	    ARCHIVE 		DESTINATION lib
	    LIBRARY		DESTINATION lib
	    PUBLIC_HEADER	DESTINATION include/${PROJECT_NAME})

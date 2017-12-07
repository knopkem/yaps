find_path(QT5_ROOT_PREFIX 
   PATHS lib/cmake/Qt5Core/Qt5CoreConfig.cmake
   HINTS "${CMAKE_PREFIX_PATH}" 
   DOC   "Qt5 root path")

if( (NOT QT5_ROOT_PREFIX) OR (NOT IS_DIRECTORY "${QT5_ROOT_PREFIX}") )
    message(FATAL_ERROR "Qt5 path not found. please set QT5_ROOT_PREFIX")
endif()

list(FIND CMAKE_PREFIX_PATH "${QT5_ROOT_PREFIX}" _index)
if(_index LESS 0)
    list(APPEND CMAKE_PREFIX_PATH "${QT5_ROOT_PREFIX}")
endif()



# Display a little status information
message(STATUS "Found Qt5 at ${QT5_ROOT_PREFIX}")


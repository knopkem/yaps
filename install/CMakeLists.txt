set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")
include(InstallRequiredSystemLibraries)

SET (IAPPS 
    "yaps" 
    )

FOREACH( APP_NAME ${IAPPS} )
if(APPLE)
    set(EXECUTABLE "${APP_NAME}.app")
elseif(WIN32)
    set(EXECUTABLE "${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
else()
    set(EXECUTABLE "${BIN_INSTALL_DIR}/${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
endif()

 if(WIN32 AND TARGET Qt5::Core)
    get_property(_Qt5_Core_LOCATION TARGET Qt5::Core PROPERTY LOCATION)
    get_filename_component(Qt_BIN_DIR "${_Qt5_Core_LOCATION}" PATH)
  endif()

  if(WIN32 AND EXISTS "${QT_QMAKE_EXECUTABLE}")
    get_filename_component(_Qt_BIN_DIR "${QT_QMAKE_EXECUTABLE}" PATH)
    if(EXISTS "${_Qt_BIN_DIR}/QtCore4.dll")
      set(Qt_BIN_DIR ${_Qt_BIN_DIR})
    endif()
  endif()

if(WIN32 OR APPLE)
    set(APPS \${CMAKE_INSTALL_PREFIX}/${EXECUTABLE} )  # paths to executables
    set(DIRS ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${Qt_BIN_DIR})   # directories to search for prerequisites
    message(STATUS "Search Libraries ${DIRS}")
    INSTALL(CODE "
       include(BundleUtilities)
       fixup_bundle(\"${APPS}\"   \"\" \"${DIRS}\"   )
        " COMPONENT Runtime)
endif()

ENDFOREACH(APP_NAME)



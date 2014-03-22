set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${BIN_INSTALL_DIR}")
include(InstallRequiredSystemLibraries)

SET (IAPPS 
    "photomover" 
    )

FOREACH( APP_NAME ${IAPPS} )
if(APPLE)
    set(EXECUTABLE "${APP_NAME}.app")
elseif(WIN32)
    set(EXECUTABLE "${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
else()
    set(EXECUTABLE "${BIN_INSTALL_DIR}/${APP_NAME}${CMAKE_EXECUTABLE_SUFFIX}")
endif()

if(WIN32 OR APPLE)
    include(DeployQt4)
    install_qt4_executable("${EXECUTABLE}" "qico")  
endif(WIN32 OR APPLE)

if(WIN32 OR APPLE)
    set(APPS \${CMAKE_INSTALL_PREFIX}/${EXECUTABLE} )  # paths to executables
    set(DIRS ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${QT_LIBRARY_DIR})   # directories to search for prerequisites
    INSTALL(CODE "
       include(BundleUtilities)
       fixup_bundle(\"${APPS}\"   \"\" \"${DIRS}\"   )
        " COMPONENT Runtime)
endif()

ENDFOREACH(APP_NAME)



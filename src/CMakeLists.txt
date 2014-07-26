include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# applying global policies
foreach(policy ${project_policies})
  if(POLICY ${policy})
    cmake_policy(SET ${policy} NEW)
  endif()
endforeach()


## #################################################################
## Sources
## #################################################################

SET(RESOURCES
    app.qrc
)

SET(HEADERS_WRAP
    mainWindow.h
    mover.h
    exifWrapper.h
    reportDlg.h
    composerDlg.h
)

SET(HEADERS 
    ${HEADERS_WRAP}
    exifData.h
    simpleLog.h
    patternFormat.h
)
 
SET(SOURCES 
    main.cpp
    mainWindow.cpp
    mover.cpp
    exifWrapper.cpp
    exifData.cpp
    reportDlg.cpp
    simpleLog.cpp
    composerDlg.cpp
    patternFormat.cpp
)
 
QT5_WRAP_CPP(HEADERS_MOC ${HEADERS_WRAP})
QT5_ADD_RESOURCES(RESOURCES_QRC ${RESOURCES})

if(WIN32)
  SET(SOURCES ${SOURCES} ${PROJECT_NAME}.rc)
ENDIF(WIN32)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

ADD_EXECUTABLE(${PROJECT_NAME} 
    ${DEPLOYMENT_SYSTEM} 
    ${HEADERS_MOC}
    ${HEADERS}
    ${SOURCES}
    ${RESOURCES_QRC}
)

qt5_use_modules(${PROJECT_NAME} Core Gui Widgets Concurrent)

# Group common files together in Visual Studio.
SOURCE_GROUP("Header Files" FILES ${HEADERS})
SOURCE_GROUP("Source Files" FILES ${SOURCES})
SOURCE_GROUP("Generated Files" FILES ${HEADERS_MOC})

install(TARGETS ${PROJECT_NAME}  RUNTIME DESTINATION  ${BIN_INSTALL_DIR} BUNDLE DESTINATION  ${BIN_INSTALL_DIR}  )

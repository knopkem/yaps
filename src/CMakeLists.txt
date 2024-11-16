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
 
QT6_WRAP_CPP(HEADERS_MOC ${HEADERS_WRAP})
QT6_ADD_RESOURCES(RESOURCES_QRC ${RESOURCES})

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

target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Concurrent)

# Group common files together in Visual Studio.
SOURCE_GROUP("Header Files" FILES ${HEADERS})
SOURCE_GROUP("Source Files" FILES ${SOURCES})
SOURCE_GROUP("Generated Files" FILES ${HEADERS_MOC})

install(TARGETS ${PROJECT_NAME}  RUNTIME DESTINATION  ${BIN_INSTALL_DIR} BUNDLE DESTINATION  ${BIN_INSTALL_DIR}  )

#deploy qt plugins
INSTALL( DIRECTORY ${QT5_ROOT_PREFIX}/plugins/platforms DESTINATION ${BIN_INSTALL_DIR} )
INSTALL( DIRECTORY ${QT5_ROOT_PREFIX}/plugins/imageformats DESTINATION ${BIN_INSTALL_DIR} )


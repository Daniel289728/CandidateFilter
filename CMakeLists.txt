cmake_minimum_required(VERSION 3.16)

project(CandidateFIlter VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the VCPKG toolchain
set(CMAKE_TOOLCHAIN_FILE "C:/Users/danie/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
# Add vcpkg's installed package directory to CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH "C:/Users/danie/vcpkg/installed/x64-windows")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
find_package(CURL REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)




set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CandidateFIlter
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        candidate.h candidate.cpp
        candidatefilter.h candidatefilter.cpp
        candidatescorer.h candidatescorer.cpp
        candidatescorer.h candidatescorer.cpp
        downloader.h downloader.cpp
        parser.h parser.cpp
        pugiconfig.hpp pugixml.hpp
        pugixml.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET CandidateFIlter APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(CandidateFIlter SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(CandidateFIlter
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(CandidateFIlter PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.CandidateFIlter)
endif()
set_target_properties(CandidateFIlter PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS CandidateFIlter
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CandidateFIlter)
endif()

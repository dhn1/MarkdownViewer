cmake_minimum_required(VERSION 3.29)

project(MarkdownViewer VERSION 0.0.6 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(STRINGS "/etc/os-release" RESULTS REGEX "^ID=(.*)")
set (DISTRO "${CMAKE_MATCH_1}")
message (STATUS "Linux distro: ${DISTRO}")

if ("${DISTRO}" STREQUAL "linuxmint")
    set(CMAKE_INSTALL_RPATH "/usr/local/qt6.11.0/lib")
endif ()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message (STATUS "Using ccache")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif(CCACHE_FOUND)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    option(ENABLE_SANITIZER "Enable sanitizer" ON)
else()
    option(ENABLE_SANITIZER "Enable sanitizer" OFF)
endif()
if (ENABLE_SANITIZER AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message (STATUS "Adress sanitizer enabled")
    add_compile_options(-fsanitize=address -fsanitize=leak -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address -fsanitize=leak -lresolv)
    set (CMAKE_CXX_FLAGS "-O0 -ggdb3")
endif()

message (STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    #add_compile_options (-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    add_compile_options (-Wall -Wextra-semi -Wextra-semi-stmt -Wmissing-variable-declarations)
    add_compile_options (-Wmissing-prototypes -Wimplicit-float-conversion)
    add_compile_options (-Wdouble-promotion -Wimplicit-int-float-conversion -Wunused-parameter )
    #add_compile_options (-Wshadow)
    #add_compile_options (-Wpadded)
    add_compile_options (-Werror=return-type)
    add_compile_options (-ffp-contract=fast)
else()
    #add_compile_options (-Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic)
    add_compile_options (-Wall -Wextra-semi -Wmissing-declarations)
    add_compile_options (-Wfloat-conversion)
    add_compile_options (-Wdouble-promotion -Wunused-parameter)
    #add_compile_options (-Wpadded -Wshadow)
    add_compile_options (-Werror=return-type)
endif()

#add_compile_options (-gsplit-dwarf)
add_link_options($<$<CONFIG:RELEASE>:-s>)

find_program (MOLD "mold")
if (MOLD)
    add_link_options ("-fuse-ld=mold")
    message (STATUS "Using mold linker")
else ()
    find_program (LLD "lld")
    if (LLD)
        add_link_options ("-fuse-ld=lld")
        message (STATUS "Using lld linker")
    endif ()
endif ()

add_subdirectory (libUI)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        MainWindow.h
        MainWindow.ui
        About.cpp About.h About.ui
)
set_property (SOURCE main.cpp PROPERTY COMPILE_DEFINITIONS VERSION="${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    Resources.qrc
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    libui)

configure_file(leak.supp ${CMAKE_BINARY_DIR}/leak.supp COPYONLY)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install desktop entry
install (
    FILES ${CMAKE_CURRENT_LIST_DIR}/Desktop/markdownviewer.desktop
    DESTINATION /usr/share/applications)

# Install icon
install (
    FILES ${CMAKE_CURRENT_LIST_DIR}/assets/MarkdownViewer.png
    DESTINATION /usr/share/app-install/icons)

# Add new mime (nethers)
install(
    FILES ${CMAKE_CURRENT_LIST_DIR}/Desktop/markdownviewer.xml
    DESTINATION /usr/share/mime/packages)

# cpack stuff
include(InstallRequiredSystemLibraries)

if ("${DISTRO}" STREQUAL "fedora")
    set (CPACK_GENERATOR "RPM")
else()
    set (CPACK_GENERATOR "DEB")
endif ()

set (CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set (CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME})
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} - Markdown Viewer")
set (CPACK_PACKAGE_VENDOR "David Netherwood")
set (CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set (CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set (CPACK_PACKAGE_CONTACT "david_netherwood@hotmail.com")

set (CPACK_DEBIAN_PACKAGE_DEPENDS "homebrewqtcore6.11.0")

set (CPACK_RPM_PACKAGE_ARCHITECTURE "x86_64")
set (CPACK_RPM_PACKAGE_GROUP "Applications/Text")
set (CPACK_RPM_PACKAGE_RELEASE "3")
include(CPack)

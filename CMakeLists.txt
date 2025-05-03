cmake_minimum_required(VERSION 3.19)
project(SearchEngineLib LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

qt_standard_project_setup()

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_library(SearchEngineLib STATIC
    ConverterJSON.h
    InvertedIndex.h
    SearchServer.h
)

target_link_libraries(SearchEngineLib
    PRIVATE
        Qt::Core
)

include(GNUInstallDirs)

install(TARGETS SearchEngineLib
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET SearchEngineLib
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)

install(SCRIPT ${deploy_script})

target_link_libraries(SearchEngineLib PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(SearchEngineLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Test)
add_subdirectory(Executable)

target_compile_definitions(SearchEngineLib PRIVATE SEARCHENGINELIB_LIBRARY)




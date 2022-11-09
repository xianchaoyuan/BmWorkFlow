cmake_minimum_required(VERSION 3.19)

project(BmWorkFlow CXX)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")

# 贝塞尔曲线测试
add_definitions(-DNODE_DEBUG_DRAWING)

if(BUILD_DEBUG_POSTFIX_D)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

find_package(Qt5
    COMPONENTS
    Core
    Widgets
    Gui
    OpenGL
)

if (NOT Qt5_FOUND)
    message(FATAL_ERROR "Qt libraries were not found.")
endif()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE CPP_HEADER_FILES
    src/*.ui
    src/*.h)

set(CPP_SOURCE_FILES
    main.cpp
    src/node.cpp
    src/nodestate.cpp
    src/nodedatamodel.cpp
    src/nodegraphicsobject.cpp
    src/nodepainter.cpp
    src/nodegeometry.cpp
    src/connection.cpp
    src/connectionstate.cpp
    src/connectionpainter.cpp
    src/connectiongeometry.cpp
    src/connectiongraphicsobject.cpp
    src/nodeconnectioninteraction.cpp
    src/datamodelregistry.cpp
    src/flowscene.cpp
    src/flowview.cpp

    # 测试
    src/bmtestloadmodel.cpp
    src/bmtestshowmodel.cpp

    src/bmtestloadwidget.cpp
    src/bmtestshowwidget.cpp
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${CPP_HEADER_FILES}
    ${CPP_SOURCE_FILES}
    ./resources/resources.qrc
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::OpenGL
)

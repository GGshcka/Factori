cmake_minimum_required(VERSION 3.28)
project(Factori)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        OpenGLWidgets
        Concurrent
        REQUIRED)
find_package(Python3 COMPONENTS Development REQUIRED)
qt_standard_project_setup()

include_directories(${Python3_INCLUDE_DIRS})

file(GLOB FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/modules/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.qrc"
)

qt_add_resources(RESOURCES resources.qrc)

add_executable(${PROJECT_NAME}
        ${FILES}
        ${RESOURCES}
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE Qt::Core
        PRIVATE Qt::Gui
        PRIVATE Qt::Widgets
        PRIVATE Qt::OpenGLWidgets
        PRIVATE Qt::Concurrent
        PRIVATE ${Python3_LIBRARIES}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)

set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
)


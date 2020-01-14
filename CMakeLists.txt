 cmake_minimum_required(VERSION 3.5)

project(noixle LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

FILE(GLOB NOIXLE_SRC
    main.cpp
    frontend/mainwindow.cpp
    frontend/mainwindow.h
    frontend/widgets/commandlineedit.h
    frontend/widgets/commandlineedit.cpp
    frontend/widgets/zoomenabledgraphicsview.h
    frontend/widgets/zoomenabledgraphicsview.cpp
    frontend/widgets/snapshotwindow.h
    frontend/widgets/snapshotwindow.cpp
    frontend/widgets/snapshotwindow.ui
    backend/imageprocessingcollection.h
    backend/imageprocessingcollection.cpp
    backend/commandparser.h
    backend/backend.cpp
    backend/backend.h
    backend/commandparser.cpp
    frontend/mainwindow.ui
)

add_executable(noixle ${NOIXLE_SRC})

target_link_libraries(noixle PRIVATE Qt5::Widgets ${OpenCV_LIBS})

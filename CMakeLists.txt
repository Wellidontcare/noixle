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
    frontend/widgets/mousetrackerpixmapitem.cpp
    frontend/widgets/mousetrackerpixmapitem.h
    frontend/widgets/snapshotwindow.ui
    frontend/widgets/statusbar.h
    frontend/widgets/statusbar.cpp
    frontend/widgets/histogramviewer.cpp
    frontend/widgets/histogramviewer.h
    backend/image_processing/imageprocessingcollection.h
    backend/image_processing/imageprocessingcollection.cpp
    backend/backend.cpp
    backend/backend.h
    backend/parser/filterparser.h
    backend/parser/filterparser.cpp
    backend/timer/timethismacro.h
    backend/image_processing/jimage.cpp
    backend/image_processing/jimage.h
    backend/statusbarinfo.h
    backend/parser/commandparser.cpp
    backend/parser/commandparser.h
    backend/parser/command.h
    backend/timer/timer.h
    backend/timer/timer.cpp
    frontend/mainwindow.ui
    frontend/widgets/helpwindow.h
    frontend/widgets/helpwindow.cpp
    frontend/widgets/helpwindow.ui
    frontend/widgets/binarizewindow.cpp
    frontend/widgets/binarizewindow.h
    frontend/widgets/binarizewindow.ui
    backend/parser/calculation_parser.cpp
    backend/parser/calculation_parser.h
    resources.qrc
)

add_executable(noixle ${NOIXLE_SRC})

target_link_libraries(noixle PRIVATE Qt5::Widgets ${OpenCV_LIBS})

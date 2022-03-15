cmake_minimum_required(VERSION 2.8)

project(BodyEdgeDetection)

find_package(OpenCV REQUIRED)

include_directories( ./src/ )
link_directories( /usr/local/lib/ )

aux_source_directory(src/ BASE_SOURCE)

set(LIBS xml2 opencv_core opencv_highgui opencv_imgproc)

add_executable (bodyEdgeD ${BASE_SOURCE})
target_link_libraries(bodyEdgeD ${LIBS})

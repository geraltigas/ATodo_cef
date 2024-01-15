cmake_minimum_required(VERSION 3.27)
project(ATodo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

set(CEF_ROOT "/home/geraltigas/SoftwareNoReg/QCefView")

# add include dir
include_directories(${CEF_ROOT}/include)
include_directories("src/")

add_executable(ATodo main.cpp
)

#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/CefViewCore/output/Release/lib
target_link_libraries(ATodo
        Qt::Core
        Qt::Gui
        Qt::Widgets
        "${CMAKE_BINARY_DIR}/libQCefView.so"
)

set(DIR_TO_COPY "${CEF_ROOT}/.build/linux.x86_64/output/Release/bin")

# Set the destination directory
# Assuming you want to copy to a directory inside your build directory
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}")

# Add a custom target for copying
add_custom_target(copy_locales ALL)

# Add a custom command to copy the directory
add_custom_command(
    TARGET ATodo PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${DIR_TO_COPY}
            ${DESTINATION_DIR}
    COMMENT "Copying ${DIR_TO_COPY} to ${DESTINATION_DIR}"
)



cmake_minimum_required(VERSION 3.22)
project(ATodo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
option(USE_SANDBOX "Enable CEF Sandbox" ON)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED
)

set(CEF_ROOT "/home/geraltigas/SoftwareNoReg/QCefView")
file(WRITE ${CMAKE_BINARY_DIR}/auto_rebuild.cpp "/* Auto Rebuild Trigger */")

# add include dir
include_directories(${CEF_ROOT}/include)
include_directories("src/")

add_executable(ATodo
        main.cpp
        ${CMAKE_BINARY_DIR}/auto_rebuild.cpp
)

#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/CefViewCore/output/Release/lib
target_link_libraries(ATodo
        "-Wl,--no-as-needed -L${CMAKE_BINARY_DIR} -lcef"
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
#        "${CEF_ROOT}/.build/linux.x86_64/output/Release/bin/libQCefView.so"
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

set(DIR_TO_COPY "${CMAKE_SOURCE_DIR}/webres")

# Set the destination directory
# Assuming you want to copy to a directory inside your build directory
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/webres")

# Add a custom target for copying
add_custom_target(copy_webrs ALL)

# Add a custom command to copy the directory
add_custom_command(
    TARGET ATodo PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${DIR_TO_COPY}
            ${DESTINATION_DIR}
    COMMENT "Copying ${DIR_TO_COPY} to ${DESTINATION_DIR}"
)



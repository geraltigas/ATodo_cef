cmake_minimum_required(VERSION 3.27)
project(ATodo)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# find x11
find_package(X11 REQUIRED)

set(CEF_ROOT "/home/geraltigas/SoftwareNoReg/cef_binary")

# add include dir
include_directories(${CEF_ROOT})
include_directories("src/")

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.h"
)

add_executable(ATodo main.cpp
        ${SOURCES}
)

#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/CefViewCore/output/Release/lib
target_link_libraries(ATodo
        Qt::Core
        Qt::Gui
        Qt::Widgets
        "${CEF_ROOT}/build/libcef_dll_wrapper/libcef_dll_wrapper.a"
#        "${CEF_ROOT}/Release/libcef.so"
        "${CMAKE_BINARY_DIR}/libcef.so"
#        import x11 lib
        ${X11_LIBRARIES}

)

# List of files to copy
set(FILES_TO_COPY
    "${CEF_ROOT}/Release/chrome-sandbox"
    "${CEF_ROOT}/Release/libcef.so"
    "${CEF_ROOT}/Release/libGLESv2.so"
    "${CEF_ROOT}/Release/libEGL.so"
    "${CEF_ROOT}/Resources/icudtl.dat"
    "${CEF_ROOT}/Resources/chrome_100_percent.pak"
    "${CEF_ROOT}/Resources/chrome_200_percent.pak"
    "${CEF_ROOT}/Resources/resources.pak"
    "${CEF_ROOT}/Release/snapshot_blob.bin"
    "${CEF_ROOT}/Release/v8_context_snapshot.bin"
    # Add more files as needed
)

# Destination directory - where the target will be built
set(DESTINATION_DIR "${CMAKE_CURRENT_BINARY_DIR}")

# Loop over each file and create a custom command
foreach(FILE_TO_COPY IN LISTS FILES_TO_COPY)
    add_custom_command(
        TARGET ATodo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${FILE_TO_COPY}
                ${DESTINATION_DIR}
        COMMENT "Copying ${FILE_TO_COPY} to executable directory"
    )
endforeach()

# Set the directory or directories to copy
set(DIR_TO_COPY "${CEF_ROOT}/Resources/locales")

# Set the destination directory
# Assuming you want to copy to a directory inside your build directory
set(DESTINATION_DIR "${CMAKE_BINARY_DIR}/locales")

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
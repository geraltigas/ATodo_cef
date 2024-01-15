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

# find x11
find_package(X11 REQUIRED)

set(CEF_ROOT "/home/geraltigas/SoftwareNoReg/cef_binary")

# add include dir
include_directories(${CEF_ROOT})
include_directories("src/")

file(GLOB_RECURSE SOURCES
        "src/*.cpp"
        "src/*.cc"
        "src/*.h"
)

add_executable(ATodo main.cpp
        ${SOURCES}
        src/cef/x_error_handler.cpp
        src/cef/x_error_handler.h
        src/cef/browser_app.h
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

#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/output/Release/bin
#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/output/Release/bin/resources
# List of files to copy
set(FILES_TO_COPY
    "${CEF_ROOT}/Release/chrome-sandbox"
#    "${CEF_ROOT}/.build/linux.x86_64/output/Release/bin/CefViewWing"
#    "${CEF_ROOT}/.build/linux.x86_64/output/Release/bin/chrome-sandbox"
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

#/home/geraltigas/SoftwareNoReg/QCefView/.build/linux.x86_64/output/Release/bin/resources
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



if(OS_MAC)
  # Shared macOS resources (menus, icons, etc).
  set(SHARED_RESOURCES_SRCS
    ../shared/resources/mac/English.lproj/InfoPlist.strings
    ../shared/resources/mac/English.lproj/MainMenu.xib
    ../shared/resources/mac/Info.plist
    ../shared/resources/mac/shared.icns
    )

  # Info.plist files used for main and helper app bundles.
  set(SHARED_INFO_PLIST ../shared/resources/mac/Info.plist)
  set(SHARED_HELPER_INFO_PLIST ../shared/resources/mac/helper-Info.plist)
elseif(OS_WINDOWS)
  # Shared Windows resources (version information, icons).
  set(SHARED_RESOURCES_SRCS
    ../shared/resources/win/big.ico
    ../shared/resources/win/resource.h
    ../shared/resources/win/shared.rc
    ../shared/resources/win/small.ico
    )

  # Manifest files used for executables.
  set(SHARED_EXE_MANIFEST ../shared/resources/win/shared.exe.manifest)
  set(SHARED_COMPATIBILITY_MANIFEST ../shared/resources/win/compatibility.manifest)
endif()


#
# Shared configuration.
#

# Support nice project/target organization in Visual Studio and Xcode.
if(OS_MACOSX OR OS_WINDOWS)
  # Enable the creation of project folders.
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)

  # Group target contents by directory.
  macro(SET_EXAMPLE_SOURCE_GROUPS srcs)
    foreach(FILE ${srcs})
      # Get the absolute directory path.
      get_filename_component(ABS_FILE "${FILE}" ABSOLUTE)
      get_filename_component(PARENT_DIR "${ABS_FILE}" DIRECTORY)

      # Remove the common directory prefix. What remains in the group.
      string(REPLACE "${CMAKE_SOURCE_DIR}/examples/" "" GROUP "${PARENT_DIR}")

      # Convert to Windows slashes.
      string(REPLACE "/" "\\" GROUP "${GROUP}")

      source_group("${GROUP}" FILES "${FILE}")
    endforeach()
  endmacro()
endif()

# Set the configuration-specific binary output directory.
if(GEN_NINJA OR GEN_MAKEFILES)
  # Force Ninja and Make to create a subdirectory named after the configuration.
  set(EXAMPLE_TARGET_OUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
  set(EXAMPLE_TARGET_OUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
endif()

# Set properties common to all example targets.
macro(SET_EXAMPLE_PROPERTIES target)
  # Output all binaries to the configuration-specific build directory.
  set_target_properties(${target} PROPERTIES
                        ARCHIVE_OUTPUT_DIRECTORY "${EXAMPLE_TARGET_OUT_DIR}"
                        RUNTIME_OUTPUT_DIRECTORY "${EXAMPLE_TARGET_OUT_DIR}"
                        LIBRARY_OUTPUT_DIRECTORY "${EXAMPLE_TARGET_OUT_DIR}")

  if(OS_MACOSX OR OS_WINDOWS)
    # Place the target in the "examples" folder in Visual Studio and Xcode.
    set_property(TARGET ${target} PROPERTY FOLDER "examples")
  endif()
endmacro()

# Set properties on an example library target.
macro(SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES target)
  SET_LIBRARY_TARGET_PROPERTIES(${target})
  SET_EXAMPLE_PROPERTIES(${target})
endmacro()

# Set properties on an example executable target.
macro(SET_EXAMPLE_EXECUTABLE_TARGET_PROPERTIES target)
  SET_EXECUTABLE_TARGET_PROPERTIES(${target})
  SET_EXAMPLE_PROPERTIES(${target})

  if(OS_LINUX)
    # Set rpath so that libraries can be placed next to the executable.
    set_target_properties(${target} PROPERTIES INSTALL_RPATH "$ORIGIN")
    set_target_properties(${target} PROPERTIES BUILD_WITH_INSTALL_RPATH TRUE)
  elseif(OS_WINDOWS)
    # Add the custom Windows manifest files to the executable.
    add_custom_command(
      TARGET ${target}
      POST_BUILD
      COMMAND "mt.exe" -nologo
              -manifest \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_EXE_MANIFEST}\"
                        \"${CMAKE_CURRENT_SOURCE_DIR}/${SHARED_COMPATIBILITY_MANIFEST}\"
              -outputresource:"${EXAMPLE_TARGET_OUT_DIR}/${target}.exe"\;\#1
      COMMENT "Adding manifest..."
      )
  endif()
endmacro()


#
# Target configuration.
#

# Static library shared by all example executable targets.
add_subdirectory(src/shared)

# Example executable targets.
add_subdirectory(src/minimal)

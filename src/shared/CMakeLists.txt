# Copyright (c) 2017 The Chromium Embedded Framework Authors. All rights
# reserved. Use of this source code is governed by a BSD-style license that
# can be found in the LICENSE file.

#
# Source files.
#

# Sources shared by all executables.
set(SHARED_COMMON_SRCS
        app_factory.h
        main_util.cc
        main_util.h
  )

# Main executable sources.
set(SHARED_SRCS
  ${SHARED_COMMON_SRCS}
        client_manager.cc
        client_manager.h
        client_util.cc
        client_util.h
        resource_util.cc
        resource_util.h
  )
set(SHARED_SRCS_LINUX
        client_util_linux.cc
        resource_util_linux.cc
)
set(SHARED_SRCS_MAC
        client_util_mac.mm
        main_mac.mm
        resource_util_mac.mm
        resource_util_posix.cc
  )
set(SHARED_SRCS_WINDOWS
        client_util_win.cc
        main_win.cc
        resource_util_win.cc
  )

macro(APPEND_PLATFORM_SOURCES name_of_list)
  if(OS_LINUX AND ${name_of_list}_LINUX)
    list(APPEND ${name_of_list} ${${name_of_list}_LINUX})
  endif()
  if(OS_POSIX AND ${name_of_list}_POSIX)
    list(APPEND ${name_of_list} ${${name_of_list}_POSIX})
  endif()
  if(OS_WINDOWS AND ${name_of_list}_WINDOWS)
    list(APPEND ${name_of_list} ${${name_of_list}_WINDOWS})
  endif()
  if(OS_MAC AND ${name_of_list}_MAC)
    list(APPEND ${name_of_list} ${${name_of_list}_MAC})
  endif()
endmacro()
APPEND_PLATFORM_SOURCES(SHARED_SRCS)

if(OS_MAC)
  # On macOS a separate helper executable is used for subprocesses.
  set(SHARED_HELPER_SRCS
    ${SHARED_COMMON_SRCS}
          process_helper_mac.cc
          process_helper_mac.h
    )
endif()


#
# Shared configuration.
#

# Target static library names.
set(SHARED_TARGET "shared")
if(OS_MAC)
  set(SHARED_HELPER_TARGET "shared_helper")
endif()

if(OS_LINUX OR OS_WINDOWS)
  # Logical target used to link the libcef library on Linux and Windows.
  # On macOS the CEF framework is loaded dynamically at startup.
  ADD_LOGICAL_TARGET("libcef_lib" "${CEF_LIB_DEBUG}" "${CEF_LIB_RELEASE}")
endif()


#
# Linux configuration.
#

if(OS_LINUX)
  # Static library target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${SHARED_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
  COPY_FILES("${SHARED_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")

  # Output a message about setting SUID permissions on the chrome-sandbox target.
  SET_LINUX_SUID_PERMISSIONS("${SHARED_TARGET}" "${EXAMPLE_TARGET_OUT_DIR}/chrome-sandbox")
endif()


#
# Mac OS X configuration.
#

if(OS_MAC)
  # Create source groups for Xcode.
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_SRCS}")
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_HELPER_SRCS}")

  # Helper static library target.
  add_library(${SHARED_HELPER_TARGET} STATIC ${SHARED_HELPER_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_HELPER_TARGET})
  add_dependencies(${SHARED_HELPER_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_HELPER_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${SHARED_HELPER_TARGET} cef_sandbox_lib)
  endif()

  # Main static library target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper "${SHARED_HELPER_TARGET}")
  target_link_libraries(${SHARED_TARGET} libcef_dll_wrapper ${CEF_STANDARD_LIBS})
endif()


#
# Windows configuration.
#

if(OS_WINDOWS)
  # Create source groups for Visual Studio.
  SET_EXAMPLE_SOURCE_GROUPS("${SHARED_SRCS}")

  # Executable target.
  add_library(${SHARED_TARGET} STATIC ${SHARED_SRCS})
  SET_EXAMPLE_LIBRARY_TARGET_PROPERTIES(${SHARED_TARGET})
  add_dependencies(${SHARED_TARGET} libcef_dll_wrapper)
  target_link_libraries(${SHARED_TARGET} libcef_lib libcef_dll_wrapper ${CEF_STANDARD_LIBS})

  if(USE_SANDBOX)
    # Logical target used to link the cef_sandbox library.
    ADD_LOGICAL_TARGET("cef_sandbox_lib" "${CEF_SANDBOX_LIB_DEBUG}" "${CEF_SANDBOX_LIB_RELEASE}")
    target_link_libraries(${SHARED_TARGET} cef_sandbox_lib ${CEF_SANDBOX_STANDARD_LIBS})
  endif()

  # Copy binary and resource files to the target output directory.
  COPY_FILES("${SHARED_TARGET}" "${CEF_BINARY_FILES}" "${CEF_BINARY_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
  COPY_FILES("${SHARED_TARGET}" "${CEF_RESOURCE_FILES}" "${CEF_RESOURCE_DIR}" "${EXAMPLE_TARGET_OUT_DIR}")
endif()

# Install script for directory: C:/Users/sebam/.cargo/registry/src/index.crates.io-6f17d22bba15001f/fltk-sys-1.4.16/cfltk/fltk

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/sebam/.cargo/registry/src/index.crates.io-6f17d22bba15001f/fltk-sys-1.4.16/cfltk/fltk/FL" USE_SOURCE_PERMISSIONS FILES_MATCHING REGEX "/[^/]*\\.[hH]$" REGEX "/fl\\_config\\.h$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE DIRECTORY FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/FL" USE_SOURCE_PERMISSIONS FILES_MATCHING REGEX "/[^/]*\\.[hH]$")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMake/FLTK-Targets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMake/FLTK-Targets.cmake"
         "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMake/FLTK-Targets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/CMake/FLTK-Targets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets-debug.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets-minsizerel.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets-relwithdebinfo.cmake")
  endif()
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/CMakeFiles/Export/df49adab93b9e0c10c64f72458b31971/FLTK-Targets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/etc/FLTKConfig.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/.cargo/registry/src/index.crates.io-6f17d22bba15001f/fltk-sys-1.4.16/cfltk/fltk/CMake/FLTK-Functions.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/CMake" TYPE FILE FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/etc/UseFLTK.cmake")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/bin/fltk-config")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/zlib/cmake_install.cmake")
  include("C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/jpeg/cmake_install.cmake")
  include("C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/png/cmake_install.cmake")
  include("C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/src/cmake_install.cmake")

endif()


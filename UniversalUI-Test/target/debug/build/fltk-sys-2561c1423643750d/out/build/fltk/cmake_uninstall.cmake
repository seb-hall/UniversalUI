if(NOT EXISTS "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/install_manifest.txt")
   message(FATAL_ERROR
      "Cannot find install manifest: \"C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/install_manifest.txt\"")
endif(NOT EXISTS "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/install_manifest.txt")

file(READ "C:/Users/sebam/RustroverProjects/UniversalUI/UniversalUI-Test/target/debug/build/fltk-sys-2561c1423643750d/out/build/fltk/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach(file ${files})
message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
   exec_program("C:/Program Files/CMake/bin/cmake.exe"
      ARGS "-E remove -f \"$ENV{DESTDIR}${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
   )
   if(NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
   endif(NOT "${rm_retval}" STREQUAL 0)
endforeach(file)

workspace "elalang"
project "elalang"
  configurations {"Debug", "Release", "lexer"}
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir "./bin/"
  links {"LLVM-15"}
  files {"**.hpp", "**.cpp"}

  filter "configurations:Debug"
    defines { "DEBUG_BUILD" }
    symbols "On"
    removefiles {"**test*.cpp", "./tests/**"}
    buildoptions {"${llvm-config --cxxflags --ldflags --system-libs --libs core}"}

  filter "configurations:Release"
    defines { "RELEASE_BUILD" }
    optimize "On"
    removefiles {"**test*.cpp", "**/tests/**"}

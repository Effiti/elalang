workspace "elalang"
  configurations {"Debug", "Release", "Testela"}
project "elalang"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir "./bin/"
  links {"LLVM-15"}

  filter "configurations:Debug"
    defines { "DEBUG_BUILD" }
    symbols "On"
    files {"**.hpp", "**.cpp"}
    removefiles {"**test*.cpp", "./tests/**"}
    buildoptions {"${llvm-config --cxxflags --ldflags --system-libs --libs core}"}

  filter "configurations:Release"
    defines { "RELEASE_BUILD" }
    optimize "On"
    files {"**.hpp", "**.cpp"}
    removefiles {"**test*.cpp", "**/tests/**"}

  filter "configurations:Testela"
    defines  { "DEBUG_BUILD" }
    files {"**.hpp", "**.cpp"}
    removefiles {"src/main.cpp"}
    links {"gtest"}
    targetname "test"

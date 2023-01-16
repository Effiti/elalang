workspace "elalang"
  configurations {"Debug", "Release", "Testela", "Scan"}
project "elalang"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  targetdir "./bin/"

  filter "configurations:Debug"
    defines { "DEBUG_BUILD" }
    symbols "On"
    files {"**.hpp", "**.cpp"}
    removefiles {"**test*.cpp", "./tests/**"}

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
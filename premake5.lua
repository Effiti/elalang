workspace "elalang"
  configurations {"Debug", "Release", "Testela"}
project "elalang"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  includedirs {"./include"}
  targetdir "./bin/"
  links {"fmt"}

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
    removefiles {"include/**test*.cpp", "include/**/tests/**"}
    links {"gtest"}
    targetname "test"


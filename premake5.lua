workspace "elalang"
  configurations {"Debug", "Release", "Testela", "Scan"}
project "elalang"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++20"
  includedirs {"./include"}
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
    removefiles {"include/**test*.cpp", "include/**/tests/**"}
    links {"gtest"}
    targetname "test"

  filter "configurations:Scan"
    defines { "DEBUG_BUILD" }
    symbols "On"
    files {"**.hpp", "**.cpp"}
    removefiles {"**test*.cpp", "./tests/**", "./src/Parser.cpp", "src/parser.hpp", "./src/Scope.hpp", "./src/Expression.hpp", "./src/Expression.cpp"}
    targetname "lexer"


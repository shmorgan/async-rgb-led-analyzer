cmake_minimum_required (VERSION 3.11)
project(S4_Led_Analyzer)

add_definitions( -DLOGIC2 )

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "Minimum supported MacOS version" FORCE)

# enable generation of compile_commands.json, helpful for IDEs to locate include files.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# custom CMake Modules are located in the cmake directory.
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ExternalAnalyzerSDK)

set(SOURCES 
src/S4LedAnalyzer.cpp
src/S4LedAnalyzer.h
src/S4LedAnalyzerResults.cpp
src/S4LedAnalyzerResults.h
src/S4LedAnalyzerSettings.cpp
src/S4LedAnalyzerSettings.h
src/S4LedHelpers.cpp
src/S4LedHelpers.h
src/S4LedSimulationDataGenerator.cpp
src/S4LedSimulationDataGenerator.h
)

add_analyzer_plugin(S4_Led_Analyzer SOURCES ${SOURCES})

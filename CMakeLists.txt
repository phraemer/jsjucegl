cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME "jsjucegl")
set(PROJECT_DESCRIPTION "Test OpenGL in JUCE")

project("${PROJECT_NAME}" DESCRIPTION "${PROJECT_DESCRIPTION}" VERSION 1.0)



include("cmake/CPM.cmake")

# If the generator supports it you will get files organised in folders in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Create a /Modules directory in the IDE with the JUCE Module code
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Show all module sources in IDE projects" ON)

CPMAddPackage(
  NAME JUCE
  GITHUB_REPOSITORY juce-framework/JUCE
  VERSION 6.1.5
  GIT_TAG 6.1.5
)

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME "James Swift"                  # Specify the name of the plugin's author
    BUNDLE_ID com.3dengineer.jsjucegl           # Bundle ID for Apple
    IS_SYNTH FALSE                              # Is this a synth or an effect?
    NEEDS_MIDI_INPUT FALSE                      # Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT FALSE                     # Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                        # Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE           # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Js3d               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE jsGL                            # A unique four-character plugin id with at least one upper-case character
    FORMATS VST3 AU                             # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PLUGIN_NAME ${PROJECT_NAME}                 # The plugin name displayed in the DAW.
    PRODUCT_NAME ${PROJECT_NAME}                # The name of the final executable, which can differ from the target name
    DESCRIPTION ${PROJECT_DESCRIPTION}          # A short description of the plugin.
)

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

juce_generate_juce_header(${PROJECT_NAME})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

if(MSVC)
  message(STATUS "Static Runtime Library")
  target_compile_options(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:Release>:/MT>        # Runtime library: Multi-threaded
    $<$<CONFIG:MinSizeRel>:/MT>     # Runtime library: Multi-threaded
    $<$<CONFIG:RelWithDebInfo>:/MT> # Runtime library: Multi-threaded
    $<$<CONFIG:Debug>:/MTd>         # Runtime library: Multi-threaded Debug
  )
else()
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)
  message(STATUS "Minimum OS X deployment version " ${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()

# Plugin Files
FILE(GLOB CPP_FILES src/*.cpp)
FILE(GLOB HPP_FILES Source/*.h)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${CPP_FILES}
    ${HPP_FILES})

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0
        DONT_SET_USING_JUCE_NAMESPACE=1
    )

set(COMMON_LIBS
  juce::juce_audio_utils
  juce::juce_dsp
  juce::juce_osc
  juce::juce_opengl
)

set(COMMON_JUCE_LINK_FLAGS
  juce::juce_recommended_config_flags
  juce::juce_recommended_lto_flags
  juce::juce_recommended_warning_flags
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE
      ${COMMON_LIBS}
  PUBLIC
      ${COMMON_JUCE_LINK_FLAGS}
)


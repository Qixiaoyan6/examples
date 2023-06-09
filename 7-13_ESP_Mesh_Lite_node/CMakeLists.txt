# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

add_compile_options(-fdiagnostics-color=always)

# if(NOT DEFINED $ENV{MDF_PATH})
# set(ENV{MDF_PATH} ${CMAKE_CURRENT_LIST_DIR}/../../../..)
# endif()
if(DEFINED ENV{RMAKER_PATH})
    set(RMAKER_PATH $ENV{RMAKER_PATH})
else()
    set(RMAKER_PATH ./components/third_party/esp-rainmaker)
endif(DEFINED ENV{RMAKER_PATH})

set(EXCLUDE_COMPONENTS "coap" "asio")
set(EXTRA_COMPONENT_DIRS ${RMAKER_PATH}/components
    ${RMAKER_PATH}/components/esp-insights/components
    "./common"
    "./components"
    "./components/third_party"
)

if(NOT DEFINED IDF_TARGET)
    set(IDF_TARGET "esp32")
endif()

set(SDKCONFIG_DEFAULTS ${CMAKE_CURRENT_LIST_DIR}/sdkconfig_defaults/sdkconfig.defaults.${IDF_TARGET})

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# include($ENV{MDF_PATH}/mdf_functions.cmake)

# Check git revision (may trigger reruns of cmake)
# #  sets MDF_VER to MDF git revision
# mdf_get_git_revision()
project(led_light)

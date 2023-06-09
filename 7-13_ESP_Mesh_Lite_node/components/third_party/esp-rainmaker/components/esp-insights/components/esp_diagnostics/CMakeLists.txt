set(srcs "src/esp_diagnostics_log_hook.c"
         "src/esp_diagnostics_utils.c")

if(CONFIG_DIAG_ENABLE_METRICS)
    list(APPEND srcs "src/esp_diagnostics_metrics.c")
    if(CONFIG_DIAG_ENABLE_HEAP_METRICS)
        list(APPEND srcs "src/esp_diagnostics_heap_metrics.c")
    endif()
    if(CONFIG_DIAG_ENABLE_WIFI_METRICS)
        list(APPEND srcs "src/esp_diagnostics_wifi_metrics.c")
    endif()
endif()

if(CONFIG_DIAG_ENABLE_VARIABLES)
    list(APPEND srcs "src/esp_diagnostics_variables.c")
    if(CONFIG_DIAG_ENABLE_NETWORK_VARIABLES)
        list(APPEND srcs "src/esp_diagnostics_network_variables.c")
    endif()
endif()

set(priv_req freertos app_update)
# esp_hw_support component was introduced in v4.3
if("${IDF_VERSION_MAJOR}.${IDF_VERSION_MINOR}" VERSION_GREATER "4.2")
    list(APPEND priv_req esp_hw_support)
endif()

idf_component_register(SRCS "${srcs}"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES ${priv_req})

set(WRAP_FUNCTIONS esp_log_write esp_log_writev)
foreach(func ${WRAP_FUNCTIONS})
     target_link_libraries(${COMPONENT_LIB} INTERFACE "-Wl,--wrap=${func}")
endforeach()

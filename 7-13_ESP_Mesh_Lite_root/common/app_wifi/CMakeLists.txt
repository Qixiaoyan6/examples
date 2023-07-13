set(OLD_PROV_MGR "${IDF_PATH}/components/wifi_provisioning/src/")

if(IDF_VERSION_MAJOR STREQUAL "4")
    set(NEW_PROV_MGR "${CMAKE_CURRENT_LIST_DIR}/wifi_prov_mgr/v4.4/manager.c")
elseif(IDF_VERSION_MAJOR STREQUAL "5")
    set(NEW_PROV_MGR "${CMAKE_CURRENT_LIST_DIR}/wifi_prov_mgr/v5.0/manager.c")
endif()

execute_process(COMMAND cp ${NEW_PROV_MGR} ${OLD_PROV_MGR} OUTPUT_VARIABLE info
        RESULT_VARIABLE exit_code
        OUTPUT_STRIP_TRAILING_WHITESPACE)

idf_component_register(SRCS "app_wifi.c"
                       INCLUDE_DIRS "."
                       REQUIRES "wifi_provisioning" "qrcode" "mesh_lite")

if(CONFIG_APP_WIFI_SHOW_DEMO_INTRO_TEXT)
    target_compile_definitions(${COMPONENT_TARGET} PRIVATE "-D RMAKER_DEMO_PROJECT_NAME=\"${CMAKE_PROJECT_NAME}\"")
endif()

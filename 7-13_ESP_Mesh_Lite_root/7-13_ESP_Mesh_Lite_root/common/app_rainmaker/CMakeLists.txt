idf_component_register(SRCS "app_rainmaker.c"
    INCLUDE_DIRS "."
    REQUIRES app_wifi app_insights app_light esp_rainmaker app_mesh_lite)

target_add_binary_data(${COMPONENT_TARGET} "server.crt" TEXT)

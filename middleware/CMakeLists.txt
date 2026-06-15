if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "src/uart_module.cpp"
            "src/sensor_module.cpp"
            "src/script_engine_module.cpp"
            "src/ble_module.cpp"
            "src/wifi_module.cpp"
            "src/mqtt_module.cpp"
            "src/modbus_module.cpp"
            "src/webserver_module.cpp"
            "src/relay_module.cpp"
        INCLUDE_DIRS "include"
        REQUIRES kernel drivers esp_wifi mqtt esp_http_server nvs_flash esp_netif esp_event
    )
else()
    add_library(takt_middleware STATIC
        src/uart_module.cpp
        src/sensor_module.cpp
        src/script_engine_module.cpp
        src/ble_module.cpp
        src/wifi_module.cpp
        src/mqtt_module.cpp
        src/modbus_module.cpp
        src/webserver_module.cpp
        src/relay_module.cpp
    )
    target_include_directories(takt_middleware PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(takt_middleware PUBLIC takt_kernel takt_drivers)
endif()

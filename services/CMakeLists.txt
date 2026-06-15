if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "src/ota_service.cpp"
            "src/telemetry_service.cpp"
            "src/config_manager.cpp"
            "src/modbus_gateway.cpp"
        INCLUDE_DIRS "include"
        REQUIRES kernel recovery middleware
    )
else()
    add_library(takt_services STATIC
        src/ota_service.cpp
        src/telemetry_service.cpp
        src/config_manager.cpp
        src/modbus_gateway.cpp
    )
    target_include_directories(takt_services PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(takt_services PUBLIC takt_kernel takt_recovery takt_middleware)
endif()

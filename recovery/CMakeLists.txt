if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "src/recovery_manager.cpp"
            "src/recovery_transports.cpp"
        INCLUDE_DIRS "include"
        REQUIRES kernel takt_boot esp_http_server
    )
    target_compile_definitions(${COMPONENT_LIB} PUBLIC TAKT_ESP32)
else()
    add_library(takt_recovery STATIC
        src/recovery_manager.cpp
        src/recovery_transports.cpp
    )
    target_include_directories(takt_recovery PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(takt_recovery PUBLIC takt_kernel takt_bootloader)
endif()

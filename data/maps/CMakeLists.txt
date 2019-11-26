file(GLOB Maps
        "*.json"
        )

    file(COPY ${Maps} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
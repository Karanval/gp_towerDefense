file(GLOB Objects
        "*.obj"
        )

    file(COPY ${Objects} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    file(COPY ${Objects} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    file(COPY ${Objects} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
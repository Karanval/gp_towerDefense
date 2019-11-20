file(GLOB Materials
        "*.mtl"
        )

    #file(COPY ${Materials} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    #file(COPY ${Materials} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    file(COPY ${Materials} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB Textures
	"*.png"
        "*.jpg"
        )

    #file(COPY ${Textures} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    #file(COPY ${Textures} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    file(COPY ${Textures} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
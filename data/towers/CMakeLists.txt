file(GLOB Textures
	"*.json"
        )
    file(COPY ${Textures} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(GLOB Sounds
        "*.wav",
	"*.json"
        )

    #file(COPY ${Sounds} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
    #file(COPY ${Sounds} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
    file(COPY ${Sounds} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
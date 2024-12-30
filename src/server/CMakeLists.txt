add_executable(${PROJECT_NAME}
	http_server.cpp
	http_server.hpp
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	Qt5::Core
	Qt5::Concurrent
	Qt5::Network
)

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_UPPER)

set(qt_PACKAGE_FOLDER ${qt_PACKAGE_FOLDER_${CMAKE_BUILD_TYPE_UPPER}})

add_custom_command(TARGET ${PROJECT_NAME}
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${qt_PACKAGE_FOLDER}/bin $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

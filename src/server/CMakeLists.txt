add_executable(${PROJECT_NAME}
	app.cpp
	app.hpp
	http_server.cpp
	http_server.hpp
	main.cpp
)

target_link_libraries(${PROJECT_NAME}
	Qt5::Core
	Qt5::Concurrent
	Qt5::Network
)

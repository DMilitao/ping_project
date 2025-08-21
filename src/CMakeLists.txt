add_library(ping_project SHARED)

target_sources(ping_project
		       PRIVATE
			   icmp.cc
			   echo.cc
			   echo_ipv6.cc
			   echo_reply.cc
			   echo_request.cc
			   echo_request_ipv6.cc
			   echo_reply_ipv6.cc
			   nclient.cc
			   nserver.cc
			   nsocket_ipv4.cc
			   nsocket_ipv6.cc
			   )

target_include_directories(ping_project
						   PRIVATE ${CMAKE_SOURCE_DIR}
						   )
						   
install(TARGETS ping_project DESTINATION lib)

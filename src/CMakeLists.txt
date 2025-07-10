add_library(ping_project SHARED)

target_sources(ping_project
		       PRIVATE
			   icmp.cc
			   )

target_include_directories(ping_project
						   PRIVATE ${CMAKE_SOURCE_DIR}
						   )
						   
install(TARGETS ping_project DESTINATION lib)

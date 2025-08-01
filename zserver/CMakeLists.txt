add_executable(server)

target_sources(server
                PRIVATE
                main.cc
                )

target_link_libraries(server
                      PRIVATE 
                      ping_project
                      )

target_include_directories(server
                           PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS ping_project DESTINATION lib)
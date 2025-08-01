add_executable(client)

target_sources(client
                PRIVATE
                main.cc
                )

target_link_libraries(client
                      PRIVATE 
                      ping_project
                      )

target_include_directories(client
                           PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS ping_project DESTINATION lib)
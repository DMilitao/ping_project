add_executable(main)

target_sources(main
                PRIVATE
                main.cc
                )

target_link_libraries(main
                      PRIVATE 
                      ping_project
                      )

target_include_directories(main
                           PRIVATE ${CMAKE_SOURCE_DIR})

install(TARGETS ping_project DESTINATION lib)
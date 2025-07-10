find_library(gtest gtest)
find_library(gmock gmock)

add_executable(test)

target_sources(test
                PRIVATE
                icmp_test.cc
                )

target_link_libraries(test
                      PRIVATE 
                      ping_project
                      gtest
                      )

target_include_directories(test
                           PRIVATE ${CMAKE_SOURCE_DIR})
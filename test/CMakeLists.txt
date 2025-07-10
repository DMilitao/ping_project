add_executable(test)

target_sources(test
                PRIVATE
                icmp_test.cc
                )

target_link_libraries(test
                      PRIVATE 
                      ping_project
                      GTest::gtest_main
                      )

target_include_directories(test
                           PRIVATE ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)
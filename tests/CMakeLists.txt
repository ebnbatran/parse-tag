add_executable(tests 
               tests.cpp
               ${CMAKE_SOURCE_DIR}/src/id3.cpp
)

target_include_directories(tests PRIVATE
                            ${CMAKE_SOURCE_DIR}/include
)

find_package(Threads)

target_link_libraries(tests PRIVATE 
                            gtest 
                            gtest_main 
                            Threads::Threads
)
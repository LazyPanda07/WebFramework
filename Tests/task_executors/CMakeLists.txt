cmake_minimum_required(VERSION 3.27.0)

project(task_executors)

add_subdirectory(CXX)
add_subdirectory(CC)
add_subdirectory(Python)
add_subdirectory(CSharp)
add_subdirectory(TaskBrokerServer)

install(DIRECTORY configs/ DESTINATION .)

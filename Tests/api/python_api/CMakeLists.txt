cmake_minimum_required(VERSION 3.27.0)

project(PythonAPI)

install(FILES server.py DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES proxy_server.py DESTINATION ${CMAKE_INSTALL_PREFIX})
cmake_minimum_required(VERSION 3.27.0)

project(PythonAPI)

install(FILES api_test.py DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES server.py DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES proxy_server.py DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES load_balancer_server.py DESTINATION ${CMAKE_INSTALL_PREFIX})

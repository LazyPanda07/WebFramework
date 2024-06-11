cmake_minimum_required(VERSION 3.27.0)

project(python_api)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/src DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/ FILES_MATCHING PATTERN "*.py")
install(FILES ${PROJECT_SOURCE_DIR}/setup.py DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/)

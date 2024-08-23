cmake_minimum_required(VERSION 3.27.0)

project(python_api)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/web_framework_api DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/ FILES_MATCHING PATTERN "*.py")
install(FILES ${PROJECT_SOURCE_DIR}/pyproject.toml DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/)
install(FILES ${PROJECT_SOURCE_DIR}/README.md DESTINATION ${CMAKE_INSTALL_PREFIX}/api/python_api/)

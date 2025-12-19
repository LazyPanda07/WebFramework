cmake_minimum_required(VERSION 3.27.0)

project(python_test)

install(FILES main.py DESTINATION .)
install(FILES hello_executor.py DESTINATION .)
install(FILES run_python_test.bat DESTINATION .)
install(FILES run_python_test.sh DESTINATION .)
install(FILES web.json DESTINATION .)

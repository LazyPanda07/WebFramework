@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

start Server.exe %WEB_FRAMEWORK_SERVER_CONFIG%

timeout 5

call Basic.exe %WEB_FRAMEWORK_SERVER_CONFIG%

timeout 600

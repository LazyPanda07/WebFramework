@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1
set FLUTTER_EXAMPLE_PATH=%2
set CURRENT_PATH=%~dp0

start DefaultHTTPSServer.exe

call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1

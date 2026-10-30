@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1
set JWT_SECRET=1

call find_fxr.bat

TaskBrokerTests.exe .\TaskBrokerServer.exe

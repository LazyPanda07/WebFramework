@echo off

set BASE_FXR_PATH=C:\Program Files\dotnet\host\fxr
set PATH=%BASE_FXR_PATH%\10.0.3\;%BASE_FXR_PATH%\10.0.2\;%PATH%

TaskBrokerTests.exe .\TaskBrokerServer.exe

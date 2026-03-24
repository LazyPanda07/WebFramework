@echo off

set BASE_FXR_PATH=C:\Program Files\dotnet\host\fxr
set PATH=%BASE_FXR_PATH%\10.0.5\;%PATH%

TaskBrokerTests.exe .\TaskBrokerServer.exe

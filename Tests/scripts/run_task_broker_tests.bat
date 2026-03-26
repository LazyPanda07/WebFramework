@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1
set "FXR_PATH="

for /d %%D in ("C:\Program Files\dotnet\host\fxr\10.*") do (
    if defined FXR_PATH (
        set "FXR_PATH=!FXR_PATH!;%%D"
    ) else (
        set "FXR_PATH=%%D"
    )
)

set PATH=%FXR_PATH%;%PATH%

TaskBrokerTests.exe .\TaskBrokerServer.exe

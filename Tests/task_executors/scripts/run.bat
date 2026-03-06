@echo off
setlocal enabledelayedexpansion

set BASE_FXR_PATH=C:\Program Files\dotnet\host\fxr
set PATH=%BASE_FXR_PATH%\10.0.3\;%BASE_FXR_PATH%\10.0.2\;%PATH%

REM Exit on errors similar to `set -e`
set ERR=0

REM Extend LD_LIBRARY_PATH
set LD_LIBRARY_PATH=%CD%;%LD_LIBRARY_PATH%

REM Start server in background
start "" /B server.exe

REM Sleep 1 second
timeout /t 1 /nobreak >nul

REM HTTP requests
curl http://127.0.0.1:64000/cxx
if errorlevel 1 set ERR=1

curl http://127.0.0.1:64000/cc
if errorlevel 1 set ERR=1

curl http://127.0.0.1:64000/python
if errorlevel 1 set ERR=1

curl http://127.0.0.1:64000/csharp
if errorlevel 1 set ERR=1

REM Wait 5 seconds
timeout /t 5 /nobreak >nul

REM Count *.txt files
set count=0
for %%f in (*.txt) do (
    set /a count+=1
)

REM Check count
if not "!count!"=="16" (
    exit /b 1
)

exit /b %ERR%

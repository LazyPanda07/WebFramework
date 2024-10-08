@echo off

start csharp_test.exe

curl http://127.0.0.1:8080 > out.json

fc reference.json out.json > nul

if errorlevel 0 goto success

if errorlevel 1 goto error

:error
exit 1

:success
exit 0

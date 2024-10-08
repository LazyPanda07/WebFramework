@echo off

start csharp_test.exe

curl http://127.0.0.1:8080 > out.json

fc reference.json out.json > nul
if errorlevel 1 goto error
else goto success

:success
exit 0

:error
exit 1

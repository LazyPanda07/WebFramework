@echo off

dir

start csharp_test.exe

curl http://127.0.0.1:8080 > out.json

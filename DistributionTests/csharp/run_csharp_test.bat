@echo off

start csharp_test.exe

curl http://127.0.0.1:8080 > out.json

certutil -hashfile reference.json MD5
certutil -hashfile out.json MD5

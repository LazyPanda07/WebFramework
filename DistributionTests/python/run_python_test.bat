@echo off

start python main.py

:loop
if not exist "start.txt" (
    ping 127.0.0.1 -n 2 > nul
    goto loop
)

curl http://127.0.0.1:8080 > out.json

certutil -hashfile reference.json MD5
certutil -hashfile out.json MD5

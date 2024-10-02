@echo off

start python main.py

ping 127.0.0.1 -n 2 > nul

curl http://127.0.0.1:8080 > out.json

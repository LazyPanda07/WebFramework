@echo off

start python main.py

curl http://127.0.0.1:8080 > out.json

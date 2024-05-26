@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

start Server.exe %WEB_FRAMEWORK_SERVER_CONFIG%
start LoadBalancerServer.exe --config load_balancer_config.json --port 9090
start LoadBalancerServer.exe --config load_balancer_config.json --port 9091 --serversHTTPS
start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9092
start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9093 --serversHTTPS

start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10000
start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10001 --serversHTTPS
start LoadBalancerServer.exe --config load_balancer_config_https.json  --type server --port 10002
start LoadBalancerServer.exe --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS

call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG%
call LoadBalancerCore.exe --port 9090
call LoadBalancerCore.exe --port 9091
call LoadBalancerCore.exe --port 9092 --useHTTPS
call LoadBalancerCore.exe --port 9093 --useHTTPS

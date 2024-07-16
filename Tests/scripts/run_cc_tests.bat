@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

start CC_Server.exe %WEB_FRAMEWORK_SERVER_CONFIG%
start DefaultHTTPSServer.exe
@REM start CC_ProxyServer.exe --config proxy_config.json --port 15000
@REM start CC_ProxyServer.exe --config proxy_config.json --port 15001 --useHTTPS
@REM start CC_LoadBalancerServer.exe --config load_balancer_config.json --port 9090
@REM start CC_LoadBalancerServer.exe --config load_balancer_config.json --port 9091 --serversHTTPS
@REM start CC_LoadBalancerServer.exe --config load_balancer_config_https.json --port 9092
@REM start CC_LoadBalancerServer.exe --config load_balancer_config_https.json --port 9093 --serversHTTPS
@REM start CC_LoadBalancerServer.exe --config load_balancer_config.json --port 9094 --custom_heuristic

@REM start CC_LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10000
@REM start CC_LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10001 --serversHTTPS
@REM start CC_LoadBalancerServer.exe --config load_balancer_config_https.json  --type server --port 10002
@REM start CC_LoadBalancerServer.exe --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS

call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1
@REM call LoadBalancerCore.exe --port 9090 || exit 1
@REM call LoadBalancerCore.exe --port 9091 || exit 1
@REM call LoadBalancerCore.exe --port 9092 --useHTTPS || exit 1
@REM call LoadBalancerCore.exe --port 9093 --useHTTPS || exit 1
@REM call LoadBalancerCore.exe --port 9094 --custom_heuristic || exit 1
@REM call ProxyCore.exe --port 15000 || exit 1
@REM call ProxyCore.exe --port 15001 --useHTTPS || exit 1

@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

set PATH=C:\Program Files\dotnet\host\fxr\8.0.22\;%PATH%

start python server.py %WEB_FRAMEWORK_SERVER_CONFIG%
start python proxy_server.py --config proxy_config.json --port 15000
start python proxy_server.py --config proxy_config.json --port 15001 --useHTTPS
start python load_balancer_server.py --config load_balancer_config.json --port 9090
start python load_balancer_server.py --config load_balancer_config.json --port 9091 --serversHTTPS
start python load_balancer_server.py --config load_balancer_config_https.json --port 9092
start python load_balancer_server.py --config load_balancer_config_https.json --port 9093 --serversHTTPS
start python load_balancer_server.py --config load_balancer_config.json --port 9094 --custom_heuristic

start python load_balancer_server.py --config load_balancer_config.json --type server --port 10000
start python load_balancer_server.py --config load_balancer_config.json --type server --port 10001 --serversHTTPS
start python load_balancer_server.py --config load_balancer_config_https.json  --type server --port 10002
start python load_balancer_server.py --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS

start DefaultHTTPSServer.exe

python api_test.py || exit 1
call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1
call LoadBalancerCore.exe --port 9090 || exit 1
call LoadBalancerCore.exe --port 9091 || exit 1
call LoadBalancerCore.exe --port 9092 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9093 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9094 --custom_heuristic || exit 1
call ProxyCore.exe --port 15000 || exit 1
call ProxyCore.exe --port 15001 --useHTTPS || exit 1

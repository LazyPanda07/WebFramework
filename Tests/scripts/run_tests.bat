set WEB_FRAMEWORK_SERVER_CONFIG=%1

call Server.exe %WEB_FRAMEWORK_SERVER_CONFIG%
REM start DefaultHTTPSServer.exe
REM start ProxyServer.exe --config proxy_config.json --port 15000
REM start ProxyServer.exe --config proxy_config.json --port 15001 --useHTTPS
REM start LoadBalancerServer.exe --config load_balancer_config.json --port 9090
REM start LoadBalancerServer.exe --config load_balancer_config.json --port 9091 --serversHTTPS
REM start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9092
REM start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9093 --serversHTTPS
REM start LoadBalancerServer.exe --config load_balancer_config.json --port 9094 --custom_heuristic
REM 
REM start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10000
REM start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10001 --serversHTTPS
REM start LoadBalancerServer.exe --config load_balancer_config_https.json  --type server --port 10002
REM start LoadBalancerServer.exe --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS
REM 
REM call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1
REM call LoadBalancerCore.exe --port 9090 || exit 1
REM call LoadBalancerCore.exe --port 9091 || exit 1
REM call LoadBalancerCore.exe --port 9092 --useHTTPS || exit 1
REM call LoadBalancerCore.exe --port 9093 --useHTTPS || exit 1
REM call LoadBalancerCore.exe --port 9094 --custom_heuristic || exit 1
REM call ProxyCore.exe --port 15000 || exit 1
REM call ProxyCore.exe --port 15001 --useHTTPS || exit 1

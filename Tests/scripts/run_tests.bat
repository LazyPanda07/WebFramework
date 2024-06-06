set WEB_FRAMEWORK_SERVER_CONFIG=%1

start Server.exe %WEB_FRAMEWORK_SERVER_CONFIG%
start DefaultHTTPSServer.exe
start ProxyServer.exe --config proxy_config.json --port 15000
start ProxyServer.exe --config proxy_config.json --port 15001 --useHTTPS
start LoadBalancerServer.exe --config load_balancer_config.json --port 9090
start LoadBalancerServer.exe --config load_balancer_config.json --port 9091 --serversHTTPS
start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9092
start LoadBalancerServer.exe --config load_balancer_config_https.json --port 9093 --serversHTTPS
start LoadBalancerServer.exe --config load_balancer_config.json --port 9094 --custom_heuristic

start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10000
start LoadBalancerServer.exe --config load_balancer_config.json --type server --port 10001 --serversHTTPS
start LoadBalancerServer.exe --config load_balancer_config_https.json  --type server --port 10002
start LoadBalancerServer.exe --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS

call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1
call LoadBalancerCore.exe --port 9090 || exit 1
call LoadBalancerCore.exe --port 9091 || exit 1
call LoadBalancerCore.exe --port 9092 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9093 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9094 --custom_heuristic || exit 1
call ProxyCore.exe --port 15000 || exit 1
call ProxyCore.exe --port 15001 --useHTTPS || exit 1

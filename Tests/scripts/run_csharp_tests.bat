@echo off

set WEB_FRAMEWORK_SERVER_CONFIG=%1

start dotnet CSharpServer.dll %WEB_FRAMEWORK_SERVER_CONFIG%
start DefaultHTTPSServer.exe
start dotnet CSharpProxyServer --config proxy_config.json --port 15000
start dotnet CSharpProxyServer --config proxy_config.json --port 15001 --useHTTPS
start dotnet CSharpLoadBalancerServer --config load_balancer_config.json --port 9090
start dotnet CSharpLoadBalancerServer --config load_balancer_config.json --port 9091 --serversHTTPS
start dotnet CSharpLoadBalancerServer --config load_balancer_config_https.json --port 9092
start dotnet CSharpLoadBalancerServer --config load_balancer_config_https.json --port 9093 --serversHTTPS
start dotnet CSharpLoadBalancerServer --config load_balancer_config.json --port 9094 --custom_heuristic
start dotnet CSharpLoadBalancerServer --config load_balancer_config.json --type server --port 10000
start dotnet CSharpLoadBalancerServer --config load_balancer_config.json --type server --port 10001 --serversHTTPS
start dotnet CSharpLoadBalancerServer --config load_balancer_config_https.json  --type server --port 10002
start dotnet CSharpLoadBalancerServer --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS

call Core.exe %WEB_FRAMEWORK_SERVER_CONFIG% || exit 1
call LoadBalancerCore.exe --port 9090 || exit 1
call LoadBalancerCore.exe --port 9091 || exit 1
call LoadBalancerCore.exe --port 9092 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9093 --useHTTPS || exit 1
call LoadBalancerCore.exe --port 9094 --custom_heuristic || exit 1
call ProxyCore.exe --port 15000 || exit 1
call ProxyCore.exe --port 15001 --useHTTPS || exit 1

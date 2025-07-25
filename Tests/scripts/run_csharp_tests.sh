#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

dotnet CSharpServer.dll ${WEB_FRAMEWORK_SERVER_CONFIG} &
./DefaultHTTPSServer &
dotnet CSharpProxyServer.dll --config proxy_config.json --port 15000 &
dotnet CSharpProxyServer.dll --config proxy_config.json --port 15001 --use-https &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config.json --port 9090 &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config.json --port 9091 --servers-https &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config_https.json --port 9092 &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config_https.json --port 9093 --servers-https &

dotnet CSharpLoadBalancerServer.dll --config load_balancer_config.json --type server --port 10000 &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config.json --type server --port 10001 --servers-https &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config_https.json  --type server --port 10002 &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config_https.json --type server --port 10003 --servers-https &
dotnet CSharpLoadBalancerServer.dll --config load_balancer_config.json --port 9094 --custom-heuristic &
sleep 1

./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
./LoadBalancerCore --port 9090
./LoadBalancerCore --port 9091
./LoadBalancerCore --port 9092 --useHTTPS
./LoadBalancerCore --port 9093 --useHTTPS
./LoadBalancerCore --port 9094 --custom_heuristic
./ProxyCore --port 15000
./ProxyCore --port 15001 --useHTTPS

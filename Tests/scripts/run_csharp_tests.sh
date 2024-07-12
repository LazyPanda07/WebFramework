#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

dotnet CSharpServer.dll ${WEB_FRAMEWORK_SERVER_CONFIG} &
./DefaultHTTPSServer &
dotnet ProxyServer.dll --config proxy_config.json --port 15000 &
dotnet ProxyServer.dll --config proxy_config.json --port 15001 --useHTTPS &
dotnet LoadBalancerServer.dll --config load_balancer_config.json --port 9090 &
dotnet LoadBalancerServer.dll --config load_balancer_config.json --port 9091 --serversHTTPS &
dotnet LoadBalancerServer.dll --config load_balancer_config_https.json --port 9092 &
dotnet LoadBalancerServer.dll --config load_balancer_config_https.json --port 9093 --serversHTTPS &
dotnet LoadBalancerServer.dll --config load_balancer_config.json --type server --port 10000 &
dotnet LoadBalancerServer.dll --config load_balancer_config.json --type server --port 10001 --serversHTTPS &
dotnet LoadBalancerServer.dll --config load_balancer_config_https.json  --type server --port 10002 &
dotnet LoadBalancerServer.dll --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
dotnet LoadBalancerServer.dll --config load_balancer_config.json --port 9094 --custom_heuristic &
sleep 1

./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
./LoadBalancerCore --port 9090
./LoadBalancerCore --port 9091
./LoadBalancerCore --port 9092 --useHTTPS
./LoadBalancerCore --port 9093 --useHTTPS
./LoadBalancerCore --port 9094 --custom_heuristic
./ProxyCore --port 15000
./ProxyCore --port 15001 --useHTTPS

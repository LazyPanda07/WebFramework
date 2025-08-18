#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

chmod +x ./CC_Server
chmod +x ./DefaultHTTPSServer
chmod +x ./CC_ProxyServer
chmod +x ./CC_LoadBalancerServer
chmod +x ./CC_API
chmod +x ./Core
chmod +x ./LoadBalancerCore
chmod +x ./ProxyCore

./CC_Server ${WEB_FRAMEWORK_SERVER_CONFIG} &
./DefaultHTTPSServer &
./CC_ProxyServer proxy_config.json 15000 &
./CC_ProxyServer proxy_config.json 15001 --useHTTPS &
./CC_LoadBalancerServer load_balancer_config.json --port 9090 &
./CC_LoadBalancerServer load_balancer_config.json --port 9091 --serversHTTPS &
./CC_LoadBalancerServer load_balancer_config_https.json --port 9092 &
./CC_LoadBalancerServer load_balancer_config_https.json --port 9093 --serversHTTPS &
./CC_LoadBalancerServer load_balancer_config.json --port 9094 --custom_heuristic &

./CC_LoadBalancerServer load_balancer_config.json --type server --port 10000 &
./CC_LoadBalancerServer load_balancer_config.json --type server --port 10001 --serversHTTPS &
./CC_LoadBalancerServer load_balancer_config_https.json  --type server --port 10002 &
./CC_LoadBalancerServer load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
sleep 1

./CC_API
./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
./LoadBalancerCore --port 9090
./LoadBalancerCore --port 9091
./LoadBalancerCore --port 9092 --useHTTPS
./LoadBalancerCore --port 9093 --useHTTPS
./LoadBalancerCore --port 9094 --custom_heuristic
./ProxyCore --port 15000
./ProxyCore --port 15001 --useHTTPS

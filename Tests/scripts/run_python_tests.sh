#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

python3 server.py ${WEB_FRAMEWORK_SERVER_CONFIG} &
./DefaultHTTPSServer &
# ./ProxyServer --config proxy_config.json --port 15000 &
# ./ProxyServer --config proxy_config.json --port 15001 --useHTTPS &
# ./LoadBalancerServer --config load_balancer_config.json --port 9090 &
# ./LoadBalancerServer --config load_balancer_config.json --port 9091 --serversHTTPS &
# ./LoadBalancerServer --config load_balancer_config_https.json --port 9092 &
# ./LoadBalancerServer --config load_balancer_config_https.json --port 9093 --serversHTTPS &
# ./LoadBalancerServer --config load_balancer_config.json --type server --port 10000 &
# ./LoadBalancerServer --config load_balancer_config.json --type server --port 10001 --serversHTTPS &
# ./LoadBalancerServer --config load_balancer_config_https.json  --type server --port 10002 &
# ./LoadBalancerServer --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
# ./LoadBalancerServer --config load_balancer_config.json --port 9094 --custom_heuristic &
sleep 1

./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
# ./LoadBalancerCore --port 9090
# ./LoadBalancerCore --port 9091
# ./LoadBalancerCore --port 9092 --useHTTPS
# ./LoadBalancerCore --port 9093 --useHTTPS
# ./LoadBalancerCore --port 9094 --custom_heuristic
# ./ProxyCore --port 15000
# ./ProxyCore --port 15001 --useHTTPS

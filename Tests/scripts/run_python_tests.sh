#!/bin/bash

# set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

python3 server.py ${WEB_FRAMEWORK_SERVER_CONFIG} &
python3 proxy_server.py --config proxy_config.json --port 15000 &
python3 proxy_server.py --config proxy_config.json --port 15001 --useHTTPS &
python3 load_balancer_server.py --config load_balancer_config.json --port 9090 &
python3 load_balancer_server.py --config load_balancer_config.json --port 9091 --serversHTTPS &
python3 load_balancer_server.py --config load_balancer_config_https.json --port 9092 &
python3 load_balancer_server.py --config load_balancer_config_https.json --port 9093 --serversHTTPS &

python3 load_balancer_server.py --config load_balancer_config.json --type server --port 10000 &
python3 load_balancer_server.py --config load_balancer_config.json --type server --port 10001 --serversHTTPS &
python3 load_balancer_server.py --config load_balancer_config_https.json  --type server --port 10002 &
python3 load_balancer_server.py --config load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
python3 load_balancer_server.py --config load_balancer_config.json --port 9094 --custom_heuristic &

./DefaultHTTPSServer &
sleep 1

python3 api_test.py
./Core ${WEB_FRAMEWORK_SERVER_CONFIG}
./LoadBalancerCore --port 9090
./LoadBalancerCore --port 9091
./LoadBalancerCore --port 9092 --useHTTPS
./LoadBalancerCore --port 9093 --useHTTPS
./LoadBalancerCore --port 9094 --custom_heuristic
./ProxyCore --port 15000
./ProxyCore --port 15001 --useHTTPS

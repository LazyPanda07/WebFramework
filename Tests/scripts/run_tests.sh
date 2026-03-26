#!/bin/bash

set -e

FXR_PATH=$(printf ":%s" /usr/share/dotnet/host/fxr/10.*)
FXR_PATH=${FXR_PATH:1}

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${FXR_PATH}:${LD_LIBRARY_PATH}

chmod +x ./Server
chmod +x ./DefaultHTTPSServer
chmod +x ./ProxyServer
chmod +x ./LoadBalancerServer
chmod +x ./CXX_API_TESTS
chmod +x ./Core
chmod +x ./LoadBalancerCore
chmod +x ./ProxyCore

# RUNTIMES variable contains list of all needed runtimes like this: --runtime python

echo "Current runtimes: ${RUNTIMES}"

./CXX_API_TESTS
./Core --server_config ${WEB_FRAMEWORK_SERVER_CONFIG} --run_arguments ./Server ${RUNTIMES}
./LoadBalancerCore --port 9090 --load_balancer_run_arguments ./LoadBalancerServer --config load_balancer_config.json ${RUNTIMES}
./LoadBalancerCore --port 9092 --load_balancer_run_arguments ./LoadBalancerServer --config load_balancer_config_https.json --useHTTPS ${RUNTIMES} # needs to initialize runtimes for https config before using it
./LoadBalancerCore --port 9091 --load_balancer_run_arguments ./LoadBalancerServer --config load_balancer_config.json --serversHTTPS ${RUNTIMES}
./LoadBalancerCore --port 9093 --load_balancer_run_arguments ./LoadBalancerServer --config load_balancer_config_https.json --serversHTTPS --useHTTPS ${RUNTIMES}
./LoadBalancerCore --port 9094 --load_balancer_run_arguments ./LoadBalancerServer --config load_balancer_config.json --custom_heuristic ${RUNTIMES}
./ProxyCore --port 15000 --load_balancer_run_arguments ./LoadBalancerServer --proxy_run_arguments ./ProxyServer ${RUNTIMES}
./ProxyCore --port 15001 --load_balancer_run_arguments ./LoadBalancerServer --proxy_run_arguments ./ProxyServer --useHTTPS ${RUNTIMES}

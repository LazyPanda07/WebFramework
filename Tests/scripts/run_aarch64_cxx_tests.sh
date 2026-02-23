#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

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

qemu-aarch64 ./CXX_API_TESTS
qemu-aarch64 ./Core --server_config ${WEB_FRAMEWORK_SERVER_CONFIG} --run_arguments "qemu-aarch64 ./Server" ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9090 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --config load_balancer_config.json ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9092 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --config load_balancer_config_https.json --useHTTPS ${RUNTIMES} # needs to initialize runtimes for https config before using it
qemu-aarch64 ./LoadBalancerCore --port 9091 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --config load_balancer_config.json --serversHTTPS ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9093 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --config load_balancer_config_https.json --serversHTTPS --useHTTPS ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9094 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --config load_balancer_config.json --custom_heuristic ${RUNTIMES}
qemu-aarch64 ./ProxyCore --port 15000 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --proxy_run_arguments "qemu-aarch64 ./ProxyServer" ${RUNTIMES}
qemu-aarch64 ./ProxyCore --port 15001 --load_balancer_run_arguments "qemu-aarch64 ./LoadBalancerServer" --proxy_run_arguments "qemu-aarch64 ./ProxyServer" --useHTTPS ${RUNTIMES}

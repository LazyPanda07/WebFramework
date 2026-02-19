#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

qemu-aarch64 ./CC_Server ${WEB_FRAMEWORK_SERVER_CONFIG} &
qemu-aarch64 ./DefaultHTTPSServer &
qemu-aarch64 ./CC_ProxyServer proxy_config.json 15000 &
qemu-aarch64 ./CC_ProxyServer proxy_config.json 15001 --useHTTPS &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config.json --port 9090 &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config.json --port 9091 --serversHTTPS &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config_https.json --port 9092 &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config_https.json --port 9093 --serversHTTPS &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config.json --port 9094 --custom_heuristic &

qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config.json --type server --port 10000 &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config.json --type server --port 10001 --serversHTTPS &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config_https.json  --type server --port 10002 &
qemu-aarch64 ./CC_LoadBalancerServer aarch64_load_balancer_config_https.json --type server --port 10003 --serversHTTPS &
sleep 1

# RUNTIMES variable contains list of all needed runtimes like this: --runtime python

echo "Current runtimes: ${RUNTIMES}"

qemu-aarch64 ./CC_API_TESTS
qemu-aarch64 ./Core --server_config ${WEB_FRAMEWORK_SERVER_CONFIG} --run_arguments "qemu-aarch64 ./CC_Server" ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9090 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --config load_balancer_config.json ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9092 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --config load_balancer_config_https.json --useHTTPS ${RUNTIMES} # needs to initialize runtimes for https config before using it
qemu-aarch64 ./LoadBalancerCore --port 9091 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --config load_balancer_config.json --serversHTTPS ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9093 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --config load_balancer_config_https.json --serversHTTPS --useHTTPS ${RUNTIMES}
qemu-aarch64 ./LoadBalancerCore --port 9094 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --config load_balancer_config.json --custom_heuristic ${RUNTIMES}
qemu-aarch64 ./ProxyCore --port 15000 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --proxy_run_arguments "qemu-aarch64 ./CC_ProxyServer" ${RUNTIMES}
qemu-aarch64 ./ProxyCore --port 15001 --load_balancer_run_arguments "qemu-aarch64 ./CC_LoadBalancerServer" --proxy_run_arguments "qemu-aarch64 ./CC_ProxyServer" --useHTTPS ${RUNTIMES}

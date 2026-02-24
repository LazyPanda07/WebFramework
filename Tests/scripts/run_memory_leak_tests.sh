#!/bin/bash

set -e

function check_memory_leak_results()
{
	local FILE_NAME=$1
	local VALGRIND_RESULT_FREED=$(cat ${FILE_NAME} | grep -Rnw -e 'All heap blocks were freed -- no leaks are possible')
	local VALGRIND_RESULT_DEFINITELY_LOST=$(cat ${FILE_NAME} | grep -Rnw -e 'definitely lost: 0 bytes')
	local VALGRIND_RESULT_INDIRECTLY_LOST=$(cat ${FILE_NAME} | grep -Rnw -e 'indirectly lost: 0 bytes')

	cat ${FILE_NAME}

	if [[ -n "${VALGRIND_RESULT_DEFINITELY_LOST}" ]];
	then
		return 0
	fi
	
	if [[ -n "${VALGRIND_RESULT_INDIRECTLY_LOST}" ]];
	then
		return 0
	fi
	
	if [[ -z "${VALGRIND_RESULT_FREED}" ]];
	then
		return 2
	fi
}

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

CORE_FILE_NAME=core_memory_leaks_result.txt
PROXY_FILE_NAME=proxy_memory_leaks_result.txt
PROXY_HTTPS_FILE_NAME=proxy_https_memory_leaks_result.txt
LOAD_BALANCER_9090_FILE_NAME=load_balancer_9090_memory_leaks_result.txt
LOAD_BALANCER_9091_FILE_NAME=load_balancer_9091_memory_leaks_result.txt
LOAD_BALANCER_9092_FILE_NAME=load_balancer_9092_memory_leaks_result.txt
LOAD_BALANCER_9093_FILE_NAME=load_balancer_9093_memory_leaks_result.txt
LOAD_BALANCER_9094_FILE_NAME=load_balancer_9094_memory_leaks_result.txt

./Core --server_config ${WEB_FRAMEWORK_SERVER_CONFIG} --run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${CORE_FILE_NAME} ./Server"
./LoadBalancerCore --port 9090 --load_balancer_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${LOAD_BALANCER_9090_FILE_NAME} ./LoadBalancerServer" --config load_balancer_config.json
./LoadBalancerCore --port 9091 --load_balancer_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${LOAD_BALANCER_9091_FILE_NAME} ./LoadBalancerServer" --config load_balancer_config.json --serversHTTPS
./LoadBalancerCore --port 9092 --load_balancer_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${LOAD_BALANCER_9092_FILE_NAME} ./LoadBalancerServer" --config load_balancer_config_https.json --useHTTPS
./LoadBalancerCore --port 9093 --load_balancer_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${LOAD_BALANCER_9093_FILE_NAME} ./LoadBalancerServer" --config load_balancer_config_https.json --serversHTTPS --useHTTPS
./LoadBalancerCore --port 9094 --load_balancer_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${LOAD_BALANCER_9094_FILE_NAME} ./LoadBalancerServer" --config load_balancer_config.json --custom_heuristic
./ProxyCore --port 15000 --load_balancer_run_arguments ./LoadBalancerServer --proxy_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${PROXY_FILE_NAME} ./ProxyServer"
./ProxyCore --port 15001 --load_balancer_run_arguments ./LoadBalancerServer --proxy_run_arguments "valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${PROXY_HTTPS_FILE_NAME} ./ProxyServer" --useHTTPS

sleep 10

check_memory_leak_results ${CORE_FILE_NAME}
check_memory_leak_results ${PROXY_FILE_NAME}
check_memory_leak_results ${PROXY_HTTPS_FILE_NAME}
check_memory_leak_results ${LOAD_BALANCER_9090_FILE_NAME}
check_memory_leak_results ${LOAD_BALANCER_9091_FILE_NAME}
check_memory_leak_results ${LOAD_BALANCER_9092_FILE_NAME}
check_memory_leak_results ${LOAD_BALANCER_9093_FILE_NAME}
check_memory_leak_results ${LOAD_BALANCER_9094_FILE_NAME}

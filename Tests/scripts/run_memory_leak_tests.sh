#!/bin/bash

set -e

function check_memory_leak_results()
{
	local VALGRIND_RESULT_FREED=$(grep -Rnw '$1' -e 'All heap blocks were freed -- no leaks are possible')
	local VALGRIND_RESULT_DEFINITELY_LOST=$(grep -Rnw '$1' -e 'definitely lost: 0 bytes')
	local VALGRIND_RESULT_INDIRECTLY_LOST=$(grep -Rnw '$1' -e 'indirectly lost: 0 bytes')

	cat $1

	if [[ -n "$VALGRIND_RESULT_DEFINITELY_LOST" ]];
	then
		exit 0
	fi
	
	if [[ -n "$VALGRIND_RESULT_INDIRECTLY_LOST" ]];
	then
		exit 0
	fi
	
	if [[ -z "$VALGRIND_RESULT_FREED" ]];
	then
		exit 2
	fi
}

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

CORE_FILE_NAME=core_memory_leaks_result.txt

valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=${CORE_FILE_NAME} ./Server ${WEB_FRAMEWORK_SERVER_CONFIG} & sleep 1

./Core ${WEB_FRAMEWORK_SERVER_CONFIG}

kill -TERM $(cat start_server.txt)

sleep 10

check_memory_leak_results ${CORE_FILE_NAME}

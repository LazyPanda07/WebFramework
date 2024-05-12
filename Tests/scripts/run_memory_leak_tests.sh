#!/bin/bash

set -e

export WEB_FRAMEWORK_SERVER_CONFIG=$1
export LD_LIBRARY_PATH=$(pwd):${LD_LIBRARY_PATH}

valgrind --max-threads=1000 --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=memory_leaks_result.txt ./Server ${WEB_FRAMEWORK_SERVER_CONFIG} & sleep 1

./Core ${WEB_FRAMEWORK_SERVER_CONFIG}

kill -TERM $(cat start_server.txt)

sleep 10

export VALGRIND_RESULT_FREED=$(grep -Rnw 'memory_leaks_result.txt' -e 'All heap blocks were freed -- no leaks are possible')
export VALGRIND_RESULT_DEFINITELY_LOST=$(grep -Rnw 'memory_leaks_result.txt' -e 'definitely lost: 0 bytes')
export VALGRIND_RESULT_INDIRECTLY_LOST=$(grep -Rnw 'memory_leaks_result.txt' -e 'indirectly lost: 0 bytes')

cat memory_leaks_result.txt

if [[ -n "${VALGRIND_RESULT_DEFINITELY_LOST}" ]];
then
	exit 0
fi

if [[ -n "${VALGRIND_RESULT_INDIRECTLY_LOST}" ]];
then
	exit 0
fi

if [[ -z "${VALGRIND_RESULT_FREED}" ]];
then
	exit 2
fi

/*
 * EmDmapTailToKafka.cpp
 *
 *  Created on: Dec 18, 2014
 *      Author: meiyuli
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
#include <libgen.h>
#include <getopt.h>
#include <pthread.h>
#include <execinfo.h>
//
#include <string.h>
#include "config/config.h"
#include "include/log/log.h"
#include "include/logstat/logstat.h"
#include "kernel/EmDmapManager.h"

void handle_sig(int signum) {
	printf("[main] Receive SIG INT: %d.\n",signum);
	exit(0);
}
// 打印内存栈信息
void sigdump(int s) {
	void *array[10];
	int size = backtrace (array, 10);
	char **stack_info = backtrace_symbols (array, size);
	fprintf(stdout,"[main] Obtained %d stack frames!\n",size);
	int i=0;
	for (i = 0; i < size; i++){
		fprintf(stdout,"[main] %s\n",stack_info[i]);
	}
	free(stack_info);
	stack_info = NULL;
	exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGPIPE, SIG_IGN);
	signal(SIGINT, handle_sig);
	signal(SIGSEGV, (__sighandler_t)sigdump);// 无效内存引用信号

	const char *conf_path = "../conf/engine.conf";
	if(argc>1) conf_path = argv[1];
	initG(conf_path);
	if(!setup_logging(G_LOG_LEVEL, G_LOG_PATH, G_LOG_IDX_PATH)) {
		printf("init log path failed.\n");
		exit(0);
	}

//	log_monit_init(G_MONITOR_PATH);

	EmDmapManager manager;
	manager.Init();
	manager.Run();
	manager.UnInit();

	uninitG();
	return 0;
}




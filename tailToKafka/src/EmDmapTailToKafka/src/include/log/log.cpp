/*
 * Log.cpp
 *
 *  Created on: 2012-2-1
 *      Author: root
 */

#include "log.h"
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAX_LOG_INDEX		10
#define MAX_LOG_SIZE		1024*1024*10 // 10M
Log g_Log;
Log* g_pLog = &g_Log;
static pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;

Log::Log():m_log_level(-1),m_log_file_path(NULL),m_log_idx_file_path(NULL),m_log_fd(-1) {
	m_curr_pid = getpid();
}

Log::~Log() {
	if(NULL!=m_log_file_path) {
		free(m_log_file_path);
		m_log_file_path = NULL;
	}
	if(NULL!=m_log_idx_file_path) {
		free(m_log_idx_file_path);
		m_log_idx_file_path = NULL;
	}
}

bool Log::InitIndex() {
	int fd = ::open(m_log_idx_file_path, O_RDONLY);
	if (fd == -1) {
		return LogIndex(0);
	}
	char buffer[32] = {0};
	int len = 32;
	int ret = read(fd, buffer, len);
	if (ret == 0) {
		return false;
	}
	m_log_idx = atoi(buffer);
	return true;
}
bool Log::LogIndex(int idx) {
	int fd = ::open(m_log_idx_file_path, O_RDWR|O_CREAT, S_IWUSR|S_IRUSR);
	if(fd == -1){
		return false;
	}

	char buffer[32];
	int maxlen = 32;
	int slen = snprintf(buffer, maxlen, "%d\n", idx);
	int retl = write(fd, buffer, slen);
	if(retl != slen) {
		close(fd);
		fd = -1;
		return false;
	}

	close(fd);
	fd = -1;
	return true;
}

long Log::GetFlSize(int fd) {
	if(fd == -1) {
		return 0;
	}
	struct stat stat;
	int ret = fstat(fd, &stat);
	if (ret < 0) {
		return 0;
	}
	return stat.st_size;
}
bool Log::OpenLogFile() {
	if(m_log_fd != -1) {
		return true;
	}
	char filename[1024];
	int len = 1024;
	snprintf(filename, len, "%s.%d", m_log_file_path, m_log_idx);
	m_log_fd = ::open(filename, O_RDWR|O_APPEND|O_CREAT, S_IWUSR|S_IRUSR);
	if(m_log_fd != -1){
		return true;
	}
	return false;
}
bool Log::SetupLogging(int log_level, char *log_file_path, char *log_idx_file_path) {
	m_log_level = log_level;

	m_log_file_path = (char *)malloc(strlen(log_file_path)+1);
	memset(m_log_file_path,0,strlen(log_file_path)+1);
	strncpy(m_log_file_path,log_file_path,strlen(log_file_path));

	m_log_idx_file_path = (char *)malloc(strlen(log_idx_file_path)+1);
	memset(m_log_idx_file_path,0,strlen(log_idx_file_path)+1);
	strncpy(m_log_idx_file_path,log_idx_file_path,strlen(log_idx_file_path));

	if(! InitIndex()) {
		printf("Log initialize index failed.[%s:%d]\n",__FILE__,__LINE__);
		return false;
	}
	return true;
}

void Log::Logg(int level, const char *fmt, ...) {
	if(NULL==m_log_file_path || -1==m_log_level) {
		//printf("Please Setup Logging first before using the logging function![%s:%d]\n",__FILE__,__LINE__);
		return ;
	}
	const char *c = ".-*#";

	if(level<m_log_level) return;

	if(m_log_fd == -1) {
		pthread_mutex_lock(&g_mutex);
		if(!OpenLogFile()) {
			printf("Log object open file failed,error:%s.[%s:%d]\n",strerror(errno),__FILE__,__LINE__);
			pthread_mutex_unlock(&g_mutex);
			return ;
		}
		pthread_mutex_unlock(&g_mutex);
	}
	time_t now = time(NULL);
	char buf[64] = {0};
	strftime(buf, 64, "%d %b %H:%M:%S",localtime(&now));

	if(LOG_TTY & 0x00){
		pthread_mutex_lock(&g_mutex);
		fprintf(stdout, "[%d] %s %c ", m_curr_pid, buf, c[level]);
		va_list ap;
		va_start(ap, fmt);
		vfprintf (stdout, fmt, ap);
		va_end(ap);
		fprintf(stdout, "\n");
		fflush(stdout);
		pthread_mutex_unlock(&g_mutex);
	}
	if(LOG_TOFILE){
		pthread_mutex_lock(&g_mutex);
		dprintf(m_log_fd, "[%d] %s %c ", m_curr_pid, buf, c[level]);
		va_list ap;
		va_start(ap, fmt);
		vdprintf (m_log_fd, fmt, ap);
		va_end(ap);
		dprintf(m_log_fd, "\n");
		fsync(m_log_fd);
		long lsize = 0;
		if((lsize=GetFlSize(m_log_fd)) >= MAX_LOG_SIZE) {
			++m_log_idx;
			if(m_log_idx>=MAX_LOG_INDEX) {
				m_log_idx = 0;
			}
			LogIndex(m_log_idx);
			close(m_log_fd);
			m_log_fd = -1;

			//delete next log file;
			char filename[256];
			int len = 256;
			snprintf(filename, len, "%s.%d", m_log_file_path, m_log_idx);
			unlink(filename);
		}
		pthread_mutex_unlock(&g_mutex);
	}
	return ;
}

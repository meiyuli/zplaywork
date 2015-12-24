/*
 * EmDmapReadFile.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */
#include <utility>
#include <signal.h>
#include "EmDmapReadFile.h"
#include "../include/WaitQuitSignal/WaitQuitSignal.h"
#include "../include/command/commandmanager.h"

extern bool g_bTaskFlag;
EmDmapReadFile::EmDmapReadFile() {
	// TODO Auto-generated constructor stub

}

EmDmapReadFile::~EmDmapReadFile() {
	// TODO Auto-generated destructor stub
}

int32_t EmDmapReadFile::Init(const string &topic, const string &filePath, const string &pattern) {
	
	pthread_mutex_init(&m_nLock, NULL);
	FileManager *pFileMgr = new FileManager();
	pFileMgr->Init(topic,filePath,pattern,&m_nLock);

	m_nFileMgr.insert(make_pair(topic, pFileMgr));
	return 0;
}

int32_t EmDmapReadFile::UnInit() {

	map<string, FileManager*>::iterator it;
	it = m_nFileMgr.begin();
	while (it != m_nFileMgr.end()) {
		delete it->second;
		it ++;
	}
	m_nFileMgr.clear();
	pthread_mutex_destroy(&m_nLock);
	return 0;
}

int32_t EmDmapReadFile::ReadFile() {

	pthread_t pid[m_nFileMgr.size()];
	map<string, FileManager*>::iterator it = m_nFileMgr.begin();
	int ncount = 0;
	while (it != m_nFileMgr.end()) {

		pthread_create(&pid[ncount++], NULL, EmDmapReadFile::Process_Files, it->second);
		it ++;
	}

	while (WaitQuitSignal::wait(g_bTaskFlag)) {
		sleep(1);
	}

	for (int i = 0; i < ncount; i++) {
		pthread_join(pid[i], NULL);
	}

	// 读取文件线程终止后，终止写数据线程
	Command *pCmd = new Command();
	pCmd->m_nCmdID = CMD_DATA_NONE;
	CCommandMgr::GetInstance()->AddOperator(pCmd);
	return 0;
}

void *EmDmapReadFile::Process_Files(void *args) {
	FileManager *pFile = reinterpret_cast<FileManager*>(args);

	pFile->ProcessFilesFromDirectory();
	return NULL;
}

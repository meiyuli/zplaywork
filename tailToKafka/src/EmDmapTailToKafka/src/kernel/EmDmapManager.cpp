/*
 * EmDmapManager.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */
#include <assert.h>
#include "../config/config.h"
#include "../include/command/commandmanager.h"
#include "../include/WaitQuitSignal/WaitQuitSignal.h"
#include "EmDmapManager.h"

EmDmapManager::EmDmapManager() {
	// TODO Auto-generated constructor stub

}

EmDmapManager::~EmDmapManager() {
	// TODO Auto-generated destructor stub
}

int32_t EmDmapManager::Init() {
	assert(G_KAFKA_TOPIC.size() > 0);

	CCommandMgr::GetInstance();
	WaitQuitSignal::init();

	deque<KafTopic>::iterator it = G_KAFKA_TOPIC.begin();
	for (;it != G_KAFKA_TOPIC.end(); it++) {
		m_nKafka.Init((*it).t_Name, (*it).t_Group);
		m_nReadFile.Init((*it).t_Name, (*it).t_Path, (*it).t_Pattern);
	}

	return 0;
}

int32_t EmDmapManager::UnInit() {
	m_nKafka.UnInit();
	m_nReadFile.UnInit();
	CCommandMgr::GetInstance()->Release();
	return 0;
}

int32_t EmDmapManager::Run() {

	// 启动读文件
	pthread_t pid;
	pthread_create(&pid,NULL,EmDmapManager::Process_WriteKafka,(void*)&m_nKafka);

	m_nReadFile.ReadFile();

	pthread_join(pid,NULL);
	// 启动写kafka
	return 0;
}

void *EmDmapManager::Process_WriteKafka(void *args) {
	EmDmapWriteKafka *pKafka = reinterpret_cast<EmDmapWriteKafka *>(args);
	assert(pKafka);

	pKafka->WriteKafka();
	return 0;
}

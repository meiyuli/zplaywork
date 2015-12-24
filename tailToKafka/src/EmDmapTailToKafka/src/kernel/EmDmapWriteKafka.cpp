/*
 * EmDmapWriteKafka.cpp
 *
 *  Created on: Dec 19, 2014
 *      Author: meiyuli
 */
#include <assert.h>
#include "FileManager.h"
#include "EmDmapWriteKafka.h"
#include "../config/config.h"
#include "../include/log/log.h"
#include "../include/command/commandmanager.h"

EmDmapWriteKafka::EmDmapWriteKafka() {
	// TODO Auto-generated constructor stub

}

EmDmapWriteKafka::~EmDmapWriteKafka() {
	// TODO Auto-generated destructor stub
}

int32_t EmDmapWriteKafka::Init(const string &topic, const string &group) {

	rdkafka_producer* pKafka = new rdkafka_producer();
	pKafka->Init(G_KAFKA_PARTITION, G_KAFKA_BROKERS, topic.c_str(), group.c_str());

	m_nTopicProducers.insert(make_pair(topic, pKafka));
	return 0;
}

int32_t EmDmapWriteKafka::UnInit() {

	map<string, rdkafka_producer*>::iterator it;
	it = m_nTopicProducers.begin();
	while (it != m_nTopicProducers.end()) {
		delete it->second;
		it ++;
	}
	m_nTopicProducers.clear();
	return 0;
}

int32_t EmDmapWriteKafka::WriteKafka() {

	CCommandMgr *pCmdMgr = CCommandMgr::GetInstance();
	assert(pCmdMgr);
	while(true) {
		Command *pCmd = pCmdMgr->GetOperatorToExcute();
		if ( NULL == pCmd ) {
//			assert(pCmd);
			continue;
		}

		if (pCmd->m_nCmdID == CMD_DATA_NONE) {
			pCmd->Remove();
			break;
		}

		switch(pCmd->m_nCmdID) {
		case CMD_DATA_DATA:
		{
			DataCommand *pDataCmd = dynamic_cast<DataCommand*>(pCmd);
			assert(pDataCmd);
			if (m_nTopicProducers[pDataCmd->m_nTopic]) {
				m_nTopicProducers[pDataCmd->m_nTopic]->Produce_Message(pDataCmd->m_nData);
			}
		}
			break;
		default:
			logg(ERROR, "error commmand id: %d", pCmd->m_nCmdID);
			break;
		}
		pCmd->Remove();
	}
	return 0;
}

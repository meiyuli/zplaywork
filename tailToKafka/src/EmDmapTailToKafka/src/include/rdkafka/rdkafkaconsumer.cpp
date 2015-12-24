/*
 * rdkafkaconsumer.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */

#include <assert.h>
#include "../../config/config.h"
#include "../../config/conf.h"
#include "../logstat/logstat.h"
#include "../command/commandmanager.h"
#include "rdkafkaconsumer.h"

rdkafka_consumer::rdkafka_consumer() : rdkafka_base() {
	// TODO Auto-generated constructor stub

}

rdkafka_consumer::~rdkafka_consumer() {
	// TODO Auto-generated destructor stub
}

int32_t rdkafka_consumer::Init(int32_t partition, const char* broker, const char* topic, const char* group) {

	int ret = 0;
	ret = rdkafka_base::Init(partition, broker, topic, group);
	return ret;
}

int32_t rdkafka_consumer::UnInit() {
	rdkafka_base::UnInit();
	return 0;
}

int32_t rdkafka_consumer::Consume_Message(int64_t offset) {
	RdKafka::Consumer *consumer = dynamic_cast<RdKafka::Consumer *>(m_handle_ptr);
	assert(consumer && m_topic_ptr);

	CCommandMgr *pCmdMgr = CCommandMgr::GetInstance();
	assert(pCmdMgr);

	int64_t start_offset = RdKafka::Topic::OFFSET_BEGINNING;
	switch(G_KAFKA_OFFSET) {
	case -2:
		start_offset = RdKafka::Topic::OFFSET_BEGINNING;
		break;
	case -1:
		start_offset = RdKafka::Topic::OFFSET_END;
		break;
	case -1000:
		start_offset = RdKafka::Topic::OFFSET_STORED;
		break;
	default:
		start_offset = offset;
		break;
	}

	//<<< Consumer message

	RdKafka::ErrorCode resp = consumer->start(m_topic_ptr, m_npartition, start_offset);
	if (resp != RdKafka::ERR_NO_ERROR) {
		// logg(LOG_ERROR, "Failed to start consumer: %s", RdKafka::err2str(resp).c_str());
		fprintf(stderr, "Failed to start consumer: %s", RdKafka::err2str(resp).c_str());
		return -1;
	}

	/**
	 * \brief Consume messages
	 */
	bool bRun = true;

	while (bRun) {
		RdKafka::Message *msg = consumer->consume(m_topic_ptr, m_npartition,
				1000);

		switch (msg->err()) {
		case RdKafka::ERR__TIMED_OUT:
			break;

		case RdKafka::ERR_NO_ERROR:
			/* Real message */
/*
			fprintf(stderr, "Read msg at offset %ld\n", msg->offset());
			printf("%.*s\n", static_cast<int>(msg->len()),
					static_cast<const char *>(msg->payload()));
*/
			do {
				char buffer[10240];
				snprintf(buffer,sizeof(buffer), "%.*s", static_cast<int>(msg->len()),
						static_cast<const char *>(msg->payload()));
/*         TODO:
				deque<string> fields;
				Conf::SafeDecimData(buffer,WORD_SEPARATOR,fields);
				if (2 != fields.size()) {
					logg(LOG_ERROR, "kafka data:%s is not correct!",buffer);
					break;
				}

				DataCommand * pDataCmd = new DataCommand(fields[0],fields[1]);
				pDataCmd->m_nCmdID = CMD_DATA_DATA;
				pCmdMgr->AddOperator(pDataCmd);
*/
			}while(0);

			if (m_topic_ptr) {
				m_topic_ptr->offset_store(m_npartition,msg->offset());
			}

			break;

		case RdKafka::ERR__PARTITION_EOF:
			/* Last message */
			// bRun = false;
			break;

		default:
			/* Errors */
			fprintf(stderr, "Consume failed: %s\n", msg->errstr().c_str());
			bRun = false;
			break;
		}

		delete msg;

		consumer->poll(0);
	}
	/*
	 * Stop consumer
	 */
	consumer->stop(m_topic_ptr, m_npartition);

	consumer->poll(1000);

	return 0;
}

rdkafka_base::RDKAFKA_TYPE rdkafka_consumer::Get_Kafka_Type() const {
	return RDKAFKA_TYPE_CONSUMER;
}

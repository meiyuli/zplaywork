/*
 * rdkafkaproducer.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */
#include <assert.h>

#include "../../config/config.h"
#include "rdkafkaproducer.h"

rdkafka_producer::rdkafka_producer()
	:rdkafka_base(){
	// TODO Auto-generated constructor stub

}

rdkafka_producer::~rdkafka_producer() {
	// TODO Auto-generated destructor stub
}

int32_t rdkafka_producer::Init(int32_t partition, const char* broker, const char* topic, const char* group) {

	int ret = 0;
	ret = rdkafka_base::Init(partition, broker, topic, group);

	return ret;
}

int32_t rdkafka_producer::UnInit() {
	rdkafka_base::UnInit();
	return 0;
}

int32_t rdkafka_producer::Produce_Message(const std::string &msg) {
	RdKafka::Producer *producer = dynamic_cast<RdKafka::Producer *>(m_handle_ptr);
	assert(producer && m_topic_ptr);
	if (msg.empty()) {
		producer->poll(0);
		return 0;
	}

	//<<< Produce message
	// int32_t partition = RdKafka::Topic::PARTITION_UA;


	RdKafka::ErrorCode resp = producer->produce(m_topic_ptr, m_npartition,
			RdKafka::Producer::MSG_COPY, //<<< copy payload
			const_cast<char *>(msg.c_str()), msg.size(),
			&msg, NULL);
	if (resp != RdKafka::ERR_NO_ERROR) {
		fprintf(stderr, "%% Produce failed: %s\n", RdKafka::err2str(resp).c_str());
		producer->poll(100);
		return -1;
	}
	/*else {
		fprintf(stdout, "%% Produced message (%ld bytes)\n", msg.size());
	}*/

	producer->poll(0);

	while(producer->outq_len() > MAX_WAIT_KAFKA_QUEUE_SIZE) {
		// fprintf(stderr, "Waiting for %d\n", producer->outq_len());
		producer->poll(100);
	}
	return 0;
}

rdkafka_base::RDKAFKA_TYPE rdkafka_producer::Get_Kafka_Type() const {
	return RDKAFKA_TYPE_PRODUCER;
}

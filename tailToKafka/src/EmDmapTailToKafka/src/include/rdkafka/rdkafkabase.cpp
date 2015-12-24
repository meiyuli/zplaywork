/*
 * rdkafkabase.cpp
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */

#include "../../config/config.h"
#include "rdkafkabase.h"

rdkafka_base::rdkafka_base() :m_npartition(-1) , m_handle_ptr(NULL), m_topic_ptr(NULL){
	// TODO Auto-generated constructor stub

}

rdkafka_base::~rdkafka_base() {
	// TODO Auto-generated destructor stub
}

int32_t rdkafka_base::Init(int32_t partition, const char* broker, const char* topic, const char* group) {
	std::string errstr;
	RdKafka::Conf *conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
	RdKafka::Conf *tconf = RdKafka::Conf::create(RdKafka::Conf::CONF_TOPIC);
	m_npartition = RdKafka::Topic::PARTITION_UA;

	switch(partition) {
	case -2:
	{
		if(tconf->set("partitioner_cb", &hash_partitioner, errstr) != RdKafka::Conf::CONF_OK) {
			printf("%s\n", errstr.c_str());
			return -1;
		}
	}
		break;
	case -1:
		// default
		break;
	default:
		m_npartition = partition;
		break;
	}

	///> 0：不压缩，1：gzip压缩方式，2：snappy压缩方式
	if (conf->set("compression.codec", "snappy", errstr) != RdKafka::Conf::CONF_OK) {
		printf("%s\n", errstr.c_str());
		return -1;
	}

	if (conf->set("metadata.broker.list", broker, errstr) != RdKafka::Conf::CONF_OK) {
		printf("%s\n", errstr.c_str());
		return -1;
	}

	if (conf->set("event_cb", &my_event_cb, errstr) != RdKafka::Conf::CONF_OK) {
		printf("%s\n", errstr.c_str());
		return -1;
	}

	if (conf->set("group.id",group,errstr) != RdKafka::Conf::CONF_OK) {
		printf("%s\n", errstr.c_str());
		return -1;
	}

	if (RDKAFKA_TYPE_PRODUCER == Get_Kafka_Type()) {

		if (conf->set("dr_cb", &my_dr_cb, errstr) != RdKafka::Conf::CONF_OK) {
			printf("%s\n", errstr.c_str());
			return -1;
		}

		m_handle_ptr = RdKafka::Producer::create(conf, errstr);
		if (!m_handle_ptr) {
			fprintf(stderr, "Failed to create producer:%s\n",errstr.c_str());
			return -1;
		}

		printf("%% Create producer %s\n", m_handle_ptr->name().c_str());
	}else if (RDKAFKA_TYPE_CONSUMER == Get_Kafka_Type()) {

		m_handle_ptr = RdKafka::Consumer::create(conf, errstr);
		if (!m_handle_ptr) {
			fprintf(stderr, "Failed to create consumer:%s\n",errstr.c_str());
			return -1;
		}

		printf("%% Create consumer %s\n", m_handle_ptr->name().c_str());
	}else {
		printf("kafka type is not recognize![%s-%d]",__FILE__,__LINE__);
		return -1;
	}

	m_topic_ptr = RdKafka::Topic::create(m_handle_ptr, topic, tconf, errstr);
	if (!m_topic_ptr) {
		fprintf(stderr, "Failed to create topic: %s\n", errstr.c_str());
		return -1;
	}
	return 0;
}

int32_t rdkafka_base::UnInit() {
	if (m_topic_ptr) {
		delete m_topic_ptr;
		m_topic_ptr = NULL;
	}
	if (m_handle_ptr) {
		delete m_handle_ptr;
		m_handle_ptr = NULL;
	}

	/*
	  Wait for RdKafka to decommission.
	* This is not strictly needed (when check outq_len() above), but
	* allows RdKafka to clean up all its resources before the application
	* exits so that memory profilers such as valgrind wont complain about
	* memory leaks.
	*/
	RdKafka::wait_destroyed(5000);

	return 0;
}

rdkafka_base::RDKAFKA_TYPE rdkafka_base::Get_Kafka_Type() const {
	return RDKAFKA_TYPE_NONE;
}

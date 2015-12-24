/*
 * rdkafkabase.h
 *
 *  Created on: Nov 24, 2014
 *      Author: meiyuli
 */

#ifndef RDKAFKABASE_H_
#define RDKAFKABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <librdkafka/rdkafkacpp.h>
#include "../md5/md5.h"
// #include "../log/log.h"

#ifndef  G_KAFKA_PARTITION
#define G_KAFKA_PARTITION -1
#endif

#ifndef G_KAFKA_BROKERS
#define G_KAFKA_BROKERS "host160:9092"
#endif

#ifndef G_KAFKA_TOPIC
#define G_KAFKA_TOPIC "outtotair"
#endif

#ifndef G_KAFKA_OFFSET
#define G_KAFKA_OFFSET -2
#endif

class MyDeliveryReportCb : public RdKafka::DeliveryReportCb {
public:
	MyDeliveryReportCb() {};
	virtual ~MyDeliveryReportCb() {};
public:
	void dr_cb(RdKafka::Message &message) {
		//logg(LOG_ERROR, "Message delivery for (%d bytes): %s",message.len(), message.errstr().c_str());
		// fprintf(stderr, "Message delivery for (%d bytes): %s", message.len(), message.errstr().c_str());
	}
};

/* Use of this partitioner is pretty pointless since no key is provided
 * in the produce() call. */
class MyHashPartitionerCb : public RdKafka::PartitionerCb {
public:
	MyHashPartitionerCb() {};
	virtual ~MyHashPartitionerCb() {};
public:
	int32_t partitioner_cb (const RdKafka::Topic *topic, const std::string *key,
                          int32_t partition_cnt, void *msg_opaque) {
		return md5_sum((unsigned char *)key->c_str(), key->size()) % partition_cnt;
	}

private:

	static inline unsigned int djb_hash (const char *str, size_t len) {
		unsigned int hash = 5381;
		for (size_t i = 0 ; i < len ; i++)
			hash = ((hash << 5) + hash) + str[i];

		return hash;
	}
};

class MyEventCb: public RdKafka::EventCb {
public:
	MyEventCb() {};
	virtual ~MyEventCb() {};
public:
	void event_cb(RdKafka::Event &event) {
		switch (event.type()) {
		case RdKafka::Event::EVENT_ERROR:
			printf("ERROR (%s): %s\n", RdKafka::err2str(event.err()).c_str(), event.str().c_str());

			if (event.err() == RdKafka::ERR__ALL_BROKERS_DOWN) {
				fprintf(stderr, "kafka all brokers down!");
				// logg(LOG_ERROR,"kafka all brokers down!");
				exit(1);
			}
			break;

		case RdKafka::Event::EVENT_STATS:
			printf("\"STATS\": %s\n", event.str().c_str());
			break;

		case RdKafka::Event::EVENT_LOG:
			fprintf(stderr, "LOG-%i-%s: %s\n", event.severity(),
					event.fac().c_str(), event.str().c_str());
			break;

		default:
			fprintf(stderr, "EVENT %d (%s):%s]\n",
					event.type(),RdKafka::err2str(event.err()).c_str(),event.str().c_str());
			break;
		}
	}
};

class rdkafka_base {
public:
	rdkafka_base();
	virtual ~rdkafka_base();
public:
	virtual int32_t Init(int32_t partition, const char* broker, const char* topic, const char* group);
	virtual int32_t UnInit();

	enum RDKAFKA_TYPE {
		RDKAFKA_TYPE_NONE = 0,
		RDKAFKA_TYPE_PRODUCER,
		RDKAFKA_TYPE_CONSUMER
	};

	virtual RDKAFKA_TYPE Get_Kafka_Type() const;
protected:
	int32_t m_npartition;
	RdKafka::Handle *m_handle_ptr;
	RdKafka::Topic	*m_topic_ptr;

	MyEventCb my_event_cb;
	MyDeliveryReportCb my_dr_cb;
	MyHashPartitionerCb hash_partitioner;
};

#endif /* RDKAFKABASE_H_ */

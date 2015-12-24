/*
 * Config.cpp
 *
 *  Created on: 2012-3-25
 *      Author: root
 */

#include "config.h"
#include "conf.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "../include/tinyxml/tinyxml.h"

static EngineConf g_stEngineConf;
EngineConf* G = &g_stEngineConf;

bool config_init(const char *conf_path) {
	printf("Loading the engine info from conf path is %s.\n", conf_path);

	TiXmlDocument doc;
	if (doc.LoadFile(conf_path)) {
		doc.Print();
	}else {
		fprintf(stderr,"can not parse xml: %s\n", conf_path);
		return false;
	}
	TiXmlElement* appElement = doc.RootElement();
	TiXmlElement* logElement = appElement->FirstChildElement();
	TiXmlElement* loglevelElement = logElement->FirstChildElement("loglevel");
	G_LOG_LEVEL = atoi(loglevelElement->Attribute("level"));

	TiXmlElement* logindexElement = loglevelElement->NextSiblingElement("logindex");
	snprintf(G_LOG_IDX_PATH, 256, "%s", logindexElement->Attribute("index"));

	TiXmlElement* logfileElement = loglevelElement->NextSiblingElement("logfile");
	snprintf(G_LOG_PATH, 256, "%s", logfileElement->Attribute("file"));

	TiXmlElement* handleElement = logElement->NextSiblingElement("handle");
	G_HANDLE_THRD = atoi(handleElement->Attribute("thread"));

	TiXmlElement* kafkaElement = logElement->NextSiblingElement("kafka");
	TiXmlElement* brokersElement = kafkaElement->FirstChildElement("brokers");
	snprintf(G_KAFKA_BROKERS, 256, "%s", brokersElement->Attribute("items"));

	TiXmlElement* partitionElement = brokersElement->NextSiblingElement("partition");
	G_KAFKA_PARTITION = atol(partitionElement->Attribute("item"));

	TiXmlElement* offsetElement = partitionElement->NextSiblingElement("offset");
	G_KAFKA_OFFSET = atoll(offsetElement->Attribute("item"));

	TiXmlElement* topicsElement = offsetElement->NextSiblingElement("topics");
	TiXmlElement* topicElement = topicsElement->FirstChildElement();

	while (topicElement) {

		const string* name = topicElement->Attribute(string("name"));
		const string* group = topicElement->Attribute(string("group"));
		TiXmlElement* sourceElement = topicElement->FirstChildElement("source");
		const string* path = sourceElement->Attribute(string("path"));
		const string* pattern = sourceElement->Attribute(string("pattern"));

		G_KAFKA_TOPIC.push_back(KafTopic(*name,*group,*path,*pattern));
		topicElement = topicElement->NextSiblingElement();
	}

	return true;
}

bool config_uninit() {

	return true;
}

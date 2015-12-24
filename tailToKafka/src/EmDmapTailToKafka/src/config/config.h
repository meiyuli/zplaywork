/*
 * Config.h
 *
 *  Created on: 2012-12-14
 *      Author: root
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define __USE_GZIP_

#include <string>
#include <deque>
using std::string;
using std::deque;

class KafTopic {
public:
	KafTopic(const string &name, const string &group, const string &path, const string &pattern)
	:t_Name(name),t_Group(group),t_Path(path),t_Pattern(pattern){};
public:
	string t_Name;
	string t_Group;
	string t_Path;
	string t_Pattern;
};

typedef struct _EngineConf{

	unsigned short	log_level;
	char		log_path[256];
	char		log_idx_path[256];

	///> 数据存储节点
	char		kafka_brokers[256];
	deque<KafTopic>	kafka_topics;
	long		kafka_partition;
	long long int	kafka_offset;

	unsigned short  thread_handle_count;

}EngineConf;

bool config_init(const char *conf_file);
bool config_uninit();
#define initG				config_init
#define uninitG				config_uninit

#define G_LOG_LEVEL			G->log_level
#define G_LOG_PATH			G->log_path
#define G_LOG_IDX_PATH			G->log_idx_path

#define G_KAFKA_BROKERS			G->kafka_brokers
#define G_KAFKA_TOPIC			G->kafka_topics
/**
 * \brief -1 random, -2 hash, >0 对应分区
 */
#define G_KAFKA_PARTITION		G->kafka_partition
/**
 * \brief -2 begin, -1 end, -1000 stored, >0 others
 */
#define G_KAFKA_OFFSET			G->kafka_offset

#define G_HANDLE_THRD			G->thread_handle_count

extern	EngineConf *G;

#endif /* CONFIG_H_ */

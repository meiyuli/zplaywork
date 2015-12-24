/*
 * command.h
 *
 *  Created on: 2014-7-5
 *      Author: YL.M
 */

#ifndef COMMAND_H_
#define COMMAND_H_
#include <assert.h>
#include <string>
#include <stdint.h>
using namespace std;

///
/// @brief
///     命令接口类
///
class Command {
public:
        int m_nCmdID;
public:
        virtual ~Command() {};
        virtual void Remove() {delete this;}
};

///
/// @brief
///     命令类型
///
const int CMD_DATA_NONE         	= 0x00000000;   ///< 空命令
const int CMD_DATA_DATA				= 0x00000001;   ///< 数据命令

class DataCommand : public Command {
public:
	DataCommand(const string &topic, const string &data)
		: m_nTopic(topic), m_nData(data){
	}
	virtual ~DataCommand() {};
public:
    ///> 主题
    string m_nTopic;
	///> 用户
    string m_nData;
};

#endif /* COMMAND_H_ */

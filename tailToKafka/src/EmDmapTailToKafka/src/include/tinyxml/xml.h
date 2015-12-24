/*
 * xml.h
 *
 *  Created on: Dec 18, 2014
 *      Author: meiyuli
 */

#ifndef SRC_INCLUDE_TINYXML_XML_H_
#define SRC_INCLUDE_TINYXML_XML_H_
#include "tinyxml.h"
#include <string>

using std::string;
class xml {
public:
	xml();
	virtual ~xml();

public:
	///< 解析xml文件
	int32_t LoadXmlFile(const string &xmlFile);

	///< 获取标签值
	int32_t GetElementValue(const string &label, string &value);

	///< 获取标签属性值
	int32_t GetElementAttributeValue(const string &label, const string &Attrib, string &value);
private:
	///< 整个xml文档
	TiXmlDocument m_nDoc;
};

#endif /* SRC_INCLUDE_TINYXML_XML_H_ */

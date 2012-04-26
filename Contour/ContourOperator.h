#pragma once

#include <vector>
class CContourOperator
{
public:
	CContourOperator(AcDbObjectIdArray &objIds);
public:
	~CContourOperator(void);
public:
	 AcDbObjectIdArray& m_objIds;
public:
	void readData(void);
};

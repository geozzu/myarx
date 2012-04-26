#pragma once

class CPolyline
{
public:
	CPolyline(AcDbObjectIdArray objIds);
public:
	~CPolyline(void);
public:
	AcDbObjectIdArray m_objIds;
public:
	void read(void);
};

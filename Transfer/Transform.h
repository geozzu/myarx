#pragma once
#include <vector>
#include <cmath>
using namespace std;
class CTransform
{
public:
	CTransform(AcDbObjectIdArray objIds);
public:
	~CTransform(void);
public:
	AcDbObjectIdArray m_objIds;
public:
	struct data{
		AcGePoint3d pt;
		double bulge;
	};
	vector<data> m_data;
	vector<data> m_resultData;
	
public:
	void read(void);
	void readPolyline(AcDbEntity *pEnt);
public:
	void write(void);
	void writeTo3DPolyline(int i);
	Acad::ErrorStatus addPolylineToModelSpace();
	//判断在那个象限
	int judge(double x,double y);
	//计算角度:0-360度
	double calJudeg(double th,int jud);
};

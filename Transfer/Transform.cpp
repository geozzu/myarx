#include "StdAfx.h"
#include "Transform.h"

CTransform::CTransform(AcDbObjectIdArray objIds):m_objIds(objIds)
{
}

CTransform::~CTransform(void)
{
}
void CTransform::readPolyline(AcDbEntity *pEnt)
{
	AcDbPolyline *pPolyline = (AcDbPolyline *)pEnt;
	int numlen = pPolyline->numVerts();
	for (int j=0;j<numlen;j++)
	{
		AcGePoint2d pt2d;
		AcGePoint3d pt3d;
		double bul;
		pPolyline->getBulgeAt(j,bul);
		pPolyline->getPointAt(j,pt2d);
		pt3d.x=pt2d.x;
		pt3d.y=pt2d.y;
		pt3d.z=pPolyline->elevation();
		data d1;
		d1.pt=pt3d;
		d1.bulge = bul;
		m_data.push_back(d1);
	}
	pPolyline->close();
}
//将数据读入
void CTransform::read(void)
{
	int len = m_objIds.length();
	for (int i = 0; i< len; i++)
	{
		AcDbEntity *pEnt = NULL;
		acdbOpenAcDbEntity(pEnt,m_objIds[i],AcDb::kForRead);
		pEnt->close();
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			readPolyline(pEnt);
		}
		else if (pEnt->isKindOf(AcDb2dPolyline::desc()))
		{
		}
		else if (pEnt->isKindOf(AcDb3dPolyline::desc()))
		{
		}
	}
}
Acad::ErrorStatus CTransform::addPolylineToModelSpace()
{
	Acad::ErrorStatus eRet;
	//获取当前数据库
	AcDbDatabase * pDb = acdbHostApplicationServices()->workingDatabase();
	//获取图块表
	AcDbBlockTable *pBlockTable;
	eRet = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
	if ( eRet != Acad::eOk)
	{
		return eRet;
	}
	//获取图块中的记录
	AcDbBlockTableRecord *pBlockTableRecord;
	eRet = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
	if ( eRet != Acad::eOk)
	{
		pBlockTable->close();
		return eRet;
	}
	pBlockTable->close();
	AcGePoint3dArray arr;
	int len = m_resultData.size();
	for (int i=0;i<len;i++)
	{
		arr.append(m_resultData[i].pt);
	}
	AcDbObjectId EntId;
	AcDb3dPolyline *pPolyline= new AcDb3dPolyline(k3dSimplePoly ,arr);
	pPolyline->setLayer(_T("DGX"));
	pPolyline->setColorIndex(1);
	eRet = pBlockTableRecord->appendAcDbEntity(EntId, pPolyline);
	if ( eRet != Acad::eOk)
	{
		pBlockTableRecord->close();
		return eRet;
	}
	pPolyline->close();

	pBlockTableRecord->close();
	return Acad::eOk;
}
int CTransform::judge(double x,double y)
{
	if (x>0)
	{
		if (y>0)
		{
			return 1;
		}
		else
			return 4;
	}
	else
	{
		if (y>0)
		{
			return 2;
		}
		else
			return 3;
	}
}
double CTransform::calJudeg(double start_th,int start_judge)
{
	double pi = 3.1415926536;
	if (2==start_judge)
	{
		start_th = pi - start_th;
	}
	else if (3==start_judge)
	{
		start_th = pi - start_th;
	}
	else if (4==start_judge)
	{
		start_th = 2*pi + start_th;
	}
	return start_th;
}
void CTransform::writeTo3DPolyline(int i)
{
	//如果有弧则开始内插:
	double bul = 0.0;
	if (bul != m_data[i].bulge)
	{
		//求出二分之一圆弧角的sin值
		double b = abs(m_data[i].bulge);
		double sin_a = 2*b/(1+b*b);
		//求出半径
		double a_b_x = m_data[i].pt.x-m_data[i+1].pt.x;
		double a_b_y = m_data[i].pt.y-m_data[i+1].pt.y;
		double l = sqrt(a_b_x*a_b_x + a_b_y*a_b_y);
		double r;
		r = l/2/sin_a;
		//求出圆心坐标.
		double arc_a = 4*atan(b);//求出圆心角
		double sin_th1,sin_th2,cos_th2;
		double cos_th1 =sin_a;
		if (m_data[i].bulge<0)
		{
			sin_th1 = -1*sqrt(1-cos_th1*cos_th1);
		}
		else
		{
			sin_th1 = sqrt(1-cos_th1*cos_th1);
		}
		sin_th2 = -1*a_b_y/l;
		cos_th2 = -1*a_b_x/l;
		double sin_th,cos_th;
		double pi = 3.1415926536;
		if (arc_a > pi)
		{
			//cos_th1 = -cos_th1;
			sin_th = sin_th2*cos_th1 -cos_th2*sin_th1;//差角公式,求出第一点与圆心连线的夹角.
			cos_th = cos_th1*cos_th2 +sin_th1*sin_th2;
		}
		else
		{
			sin_th = sin_th1*cos_th2 +cos_th1*sin_th2;//和角公式,求出第一点与圆心连线的夹角.
			cos_th = cos_th1*cos_th2 -sin_th1*sin_th2;
		}
		
		double cir_x,cir_y;
		cir_x = m_data[i].pt.x + r*cos_th;
		cir_y = m_data[i].pt.y + r*sin_th;
		//积分来插值,
		double start_th,end_th;//起始和终点的极坐标,依次来积分,以圆心为极坐标坐标原点.
		double start_x,start_y,end_x,end_y;
		start_x = m_data[i].pt.x-cir_x;
		start_y = m_data[i].pt.y-cir_y;
		end_x = m_data[i+1].pt.x-cir_x;
		end_y =m_data[i+1].pt.y-cir_y;
		int start_judge,end_judge;
		start_judge = judge(start_x,start_y);
		end_judge =judge(end_x,end_y);
		start_th= asin((m_data[i].pt.y-cir_y)/r);
		start_th = calJudeg(start_th,start_judge);
		end_th = asin((m_data[i+1].pt.y-cir_y)/r);
		end_th = calJudeg(end_th,end_judge);
		double step = arc_a/16;
		if (m_data[i].bulge > 0)
		{
			if (start_th>end_th )
			{
				for (double th_i=start_th;th_i < 2*pi;th_i+=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
				for (double th_i =0;th_i<end_th;th_i+=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
			}
			else
			{
				for (double th_i=start_th;th_i < end_th;th_i+=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
			}
		}
		//逆向
		else
		{
			if (start_th<end_th )
			{
				for (double th_i=start_th;th_i > 0;th_i-=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
				for (double th_i =2*pi;th_i>end_th;th_i-=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
			}
			else
			{
				for (double th_i=start_th;th_i > end_th;th_i-=step)
				{
					double in_x = cir_x + r*cos(th_i);
					double in_y = cir_y + r*sin(th_i);
					data data_in2;
					data_in2.bulge=0;
					data_in2.pt.x=in_x;
					data_in2.pt.y=in_y;
					data_in2.pt.z=0;
					m_resultData.push_back(data_in2);
				}
			}

		}
		
	}
}
void CTransform::write(void)
{
	int len = m_data.size();
	for (int i=0;i<len-1;i++)
	{
		m_resultData.push_back(m_data[i]);
		writeTo3DPolyline(i);
	}
	addPolylineToModelSpace();
}

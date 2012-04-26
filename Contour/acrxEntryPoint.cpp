// (C) Copyright 2002-2005 by Autodesk, Inc. 
//
// Permission to use, copy, modify, and distribute this software in
// object code form for any purpose and without fee is hereby granted, 
// provided that the above copyright notice appears in all copies and 
// that both that copyright notice and the limited warranty and
// restricted rights notice below appear in all supporting 
// documentation.
//
// AUTODESK PROVIDES THIS PROGRAM "AS IS" AND WITH ALL FAULTS. 
// AUTODESK SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTY OF
// MERCHANTABILITY OR FITNESS FOR A PARTICULAR USE.  AUTODESK, INC. 
// DOES NOT WARRANT THAT THE OPERATION OF THE PROGRAM WILL BE
// UNINTERRUPTED OR ERROR FREE.
//
// Use, duplication, or disclosure by the U.S. Government is subject to 
// restrictions set forth in FAR 52.227-19 (Commercial Computer
// Software - Restricted Rights) and DFAR 252.227-7013(c)(1)(ii)
// (Rights in Technical Data and Computer Software), as applicable.
//

//-----------------------------------------------------------------------------
//----- acrxEntryPoint.h
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "ContourOperator.h"
#include <vector>
using namespace std;

//����ÿ���ߵ���������
struct lineData 
{
	AcDbObjectId id;
	vector<AcGePoint3d> points;
	//0��ʾû�нڵ���ͬ,1��ʾ��ƥ���������������.
	bool flag ; 
	//�Ƿ��Ѿ�����������.
	bool isAdd;
};
//�洢�����ߵ�����
vector<lineData> lines;

vector<AcGePoint3d> newPoints;

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CContourApp : public AcRxArxApp {

public:
	CContourApp () : AcRxArxApp () {

	}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		// TODO: Load dependencies here

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
		
		// TODO: Add your initialization code here

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		// TODO: Add your code here

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	static Acad::ErrorStatus addPolylineToModelSpace()
	{
		Acad::ErrorStatus eRet;
		//��ȡ��ǰ���ݿ�
		AcDbDatabase * pDb = acdbHostApplicationServices()->workingDatabase();
		//��ȡͼ���
		AcDbBlockTable *pBlockTable;
		eRet = pDb->getBlockTable(pBlockTable, AcDb::kForRead);
		if ( eRet != Acad::eOk)
		{
			return eRet;
		}
		//��ȡͼ���еļ�¼
		AcDbBlockTableRecord *pBlockTableRecord;
		eRet = pBlockTable->getAt(ACDB_MODEL_SPACE, pBlockTableRecord, AcDb::kForWrite);
		if ( eRet != Acad::eOk)
		{
			pBlockTable->close();
			return eRet;
		}
		pBlockTable->close();
		//��ʵ�����ӵ���¼����.
		int len = lines.size();
		for (int i=0;i<len;i++)
		{
			//����Ǻϲ�����߱��������е㲢����array
			if (lines[i].isAdd)
			{
				
				AcDbObjectId EntId;
				int len1 = lines[i].points.size();
				AcGePoint3dArray arr;
				for (int j =0;j<len1;j++)
				{
					//AcDb3dPolylineVertex pPolylineVertex(lines[i].points[j]);
					//pPolyline->appendVertex(EntId,&pPolylineVertex);
					arr.append(lines[i].points[j]);
				}
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
			}
		}
		
		pBlockTableRecord->close();
		return Acad::eOk;
	}

	static void lineRead(AcDbEntity *obj,AcDbObjectId id){
		lineData line;
		AcDbLine *pLine = (AcDbLine *) obj;
		AcGePoint3d startPoint,endPoint;
		startPoint= pLine->startPoint();
		endPoint = pLine->endPoint();
		line.id=id;
		line.points.push_back(startPoint);
		line.points.push_back(endPoint);
		line.flag =false;
		line.isAdd = false;
		lines.push_back(line);
	}
	static void PolylineRead(AcDbEntity *obj,AcDbObjectId id){
		lineData line;
		AcDbPolyline *pLine = (AcDbPolyline *) obj;
		int len = pLine->numVerts();
		for (int i = 0 ;i< len;i++)
		{
			AcGePoint3d pt;
			pLine->getPointAt(i,pt);
			line.points.push_back(pt);

		}
		line.id=id;
		line.flag =false;
		line.isAdd = false;
		lines.push_back(line);
	}
	static Acad::ErrorStatus readData(AcDbObjectIdArray objIds)
	{
		Acad::ErrorStatus es;
		//ȡ��ѡ�񼯵ĳ���
		long len = objIds.length();
		if (len == 0)
		{
			acutPrintf(_T("δѡ��ʵ��,�˳�."));
			es = Acad::eOk;
			return es;
		}
		//����ѡ���е�ʵ��,�����
		for (int i =0;i<len;i++)
		{
			ads_name entres;
			AcDbEntity *obj;
			
			es = acdbOpenAcDbEntity(obj, objIds[i], AcDb::kForRead, true);
			if (es != Acad::eOk) {
				return es;
			}
			obj->close();
			if (obj->isKindOf(AcDbLine::desc()))
			{
				lineRead(obj,objIds[i]);
			}
			else if (obj->isKindOf(AcDbPolyline::desc()))
			{
				PolylineRead(obj,objIds[i]);
			}
		}
		return es;
	}
	//�ж��Ƿ�����ͬ�ĵ�.
	//0��ʾ��,1��ʾͷͷ,2��ʾͷβ,3βͷ,4ββ
	static int comparePoint(lineData l1,lineData l2)
	{
		AcGePoint3d pt1b = *l1.points.begin();
		AcGePoint3d pt1e = *(--l1.points.end());
		AcGePoint3d pt2b = *l2.points.begin();
		AcGePoint3d pt2e = *(--l2.points.end());
		if (pt1b==pt2b)
		{
			return 1;
		} 
		else if (pt1b==pt2e)
		{
			return 2;
		}
		else if(pt1e==pt2b)
		{
			return 3;
		}
		else if (pt1e==pt2e)
		{
			return 4;
		}
		return 0;
	}
	static void checkData()
	{
		vector< AcGePoint3d> points;
		for (int i = 0;i<lines.size();i++)
		{
			//����ڵ��Ѿ���ƥ��������
			for (int j = i+1;j<lines.size();j++)
			{
				if (lines[j].flag)
				{
					continue;
				}
				int cmp =0;
				cmp = comparePoint(lines[i],lines[j]);
				if(cmp!=0)
				{
					lines[i].flag = true;
					//head<==>head
					if(1==cmp)
					{
						lines[i].points.erase(lines[i].points.begin());
						lines[i].points.insert(lines[i].points.begin(),lines[j].points.rbegin(),lines[j].points.rend());
					}
					//head<==>end
					else if (2==cmp)
					{
						lines[i].points.erase(lines[i].points.begin());
						lines[i].points.insert(lines[i].points.begin(),lines[j].points.begin(),lines[j].points.end());
					}
					//end<==>head
					else if (3==cmp)
					{
						lines[i].points.pop_back();
						lines[i].points.insert(lines[i].points.end(),lines[j].points.rbegin(),lines[j].points.rend());
					}
					//end<==>end
					else if (4==cmp)
					{
						lines[i].points.pop_back();
						lines[i].points.insert(lines[i].points.end(),lines[j].points.begin(),lines[j].points.end());
					}
					lines[i].flag=true;
					lines[i].isAdd = true;
					lines[j].flag=true;
					i--;
					break;
				}
			}
		}
	}

public:

	// - Contour.syj_dgx command (do not rename)
	static void Contoursyj_dgx(void)
	{
		// Add your code for command Contour.syj_dgx here
		ads_name ss;
		resbuf rb;
		AcDbObjectIdArray objIds = NULL;
		acedSSGet(NULL,NULL,NULL,NULL,ss);
		long len;
		//ȡ��ѡ�񼯵ĳ���
		acedSSLength(ss,&len);
		//����ѡ���е�ʵ��,
		for (int i =0;i<len;i++)
		{
			ads_name entres;
			AcDbObjectId objId;
			//ȡ��ѡ����ʵ�������ads_name
			acedSSName(ss,i,entres);
			//ȡ��ʵ���ObjectId
			acdbGetObjectId(objId,entres);
			objIds.append(objId);
		}
		readData(objIds);
		checkData();
		//writeData();
		addPolylineToModelSpace();
		acedSSFree(ss);
		lines.clear();
	}

} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CContourApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CContourApp, Contour, syj_dgx, syj_dgx, ACRX_CMD_TRANSPARENT, NULL)

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

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CArxProject2App : public AcRxArxApp {

public:
	CArxProject2App () : AcRxArxApp () {}

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

private:

	static Acad::ErrorStatus addEntityToModelSpace(AcDbEntity * pEnt, AcDbObjectId & EntId)
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
		eRet = pBlockTableRecord->appendAcDbEntity(EntId, pEnt);
		if ( eRet != Acad::eOk)
		{
			pBlockTableRecord->close();
			return eRet;
		}
		pBlockTableRecord->close();
		return Acad::eOk;
	}

	static Acad::ErrorStatus drawCircle(ads_point ad)
	{
		Acad::ErrorStatus eRet;
		double x = ad[0];
		double y = ad[1];
		double z = ad[2];
		AcGePoint3d center(x,y,z);
		AcGeVector3d normal(0.0,0.0,2.0);
		AcDbCircle *pCirc = new AcDbCircle(center,normal,2.0);
		AcDbObjectId circleId;

		eRet = addEntityToModelSpace(pCirc,circleId);
		if ( eRet != Acad::eOk)
		{
			pCirc->close();
			return eRet;
		}
		// ����ص��������޷���ʾ
		pCirc->close();
		return eRet;
	}
	static Acad::ErrorStatus drawArc()
	{
		ads_point ad;
		acedGetPoint(NULL,_T("����Բ��:\n"),ad);
		Acad::ErrorStatus eRet;
		double x = ad[0];
		double y = ad[1];
		double z = ad[2];
		AcGePoint3d center(x,y,z);
		ads_real radius,startAngle,endAngle;
		acedGetReal(_T("����뾶"),&radius);
		acedGetReal(_T("������ʼ�Ƕ�"),&startAngle);
		acedGetReal(_T("������ֹ�Ƕ�"),&endAngle);
		AcDbArc *pArc = new AcDbArc(center,radius,startAngle,endAngle);
		AcDbObjectId arcId;

		eRet = addEntityToModelSpace(pArc,arcId);
		if ( eRet != Acad::eOk)
		{
			pArc->close();
			return eRet;
		}
		// ����ص��������޷���ʾ
		pArc->close();
		return eRet;
	}
	static Acad::ErrorStatus drawPoint(ads_point ad)
	{
		Acad::ErrorStatus eRet;
		double x = ad[0];
		double y = ad[1];
		double z = ad[2];
		AcGePoint3d center(x,y,z);
		AcDbPoint *pPoint= new AcDbPoint(center);
		AcDbObjectId pointId;

		eRet = addEntityToModelSpace(pPoint,pointId);

		if ( eRet != Acad::eOk)
		{
			pPoint->close();
			return eRet;
		}
		// ����ص��������޷���ʾ
		pPoint->close();
		return eRet;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	*drawLine ����������Ϳ���¼
	*/
	static Acad::ErrorStatus drawLine(ads_point pt1,ads_point pt2)
	{
		Acad::ErrorStatus eRet;
		AcGePoint3d startPt(pt1[0],pt1[1],pt1[2]);
		AcGePoint3d endPt(pt2[0],pt2[1],pt2[2]);
		AcDbLine  *pLine = new AcDbLine(startPt,endPt);
		AcDbObjectId lineId;

		eRet = addEntityToModelSpace(pLine,lineId);
		if ( eRet != Acad::eOk)
		{
			pLine->close();
			return eRet;
		}
		// ����ص��������޷���ʾ
		pLine->close();
		return eRet;
	}
	//////////////////////////////////////////////////////////////////////////
	/*
	 * drawPolyLine
	*/
	static Acad::ErrorStatus drawPolyLine()
	{
		Acad::ErrorStatus eRet;
		AcDbObjectId objId;
		int index =2;
		ads_point ptStart;
		acedGetPoint(NULL,_T("\n�������һ��:"),ptStart);

		ads_point ptPrivious , ptCurrent;
		acdbPointSet(ptStart,ptPrivious);
		while (acedGetPoint(ptPrivious,_T("\n������һ��:"),ptCurrent)==RTNORM)
		{
			if(index==2)
			{
				AcDbPolyline *pPolyline = new AcDbPolyline(2);
				AcGePoint2d ptGe1,ptGe2;
				ptGe1[X]=ptPrivious[X];
				ptGe1[Y] = ptPrivious[Y];
				ptGe2[X]=ptCurrent[X];
				ptGe2[Y] = ptCurrent[Y];
				pPolyline->addVertexAt(0,ptGe2);
				pPolyline->addVertexAt(1,ptGe1);
				eRet = addEntityToModelSpace( pPolyline,objId);
				if ( eRet != Acad::eOk)
				{
					return eRet;
				}
				pPolyline->close();
			}
			else if (index>2)
			{
				AcDbPolyline *pPolyline ;
				AcDbEntity *pEntity;
				acdbOpenAcDbEntity(pEntity,objId,AcDb::kForWrite);
				//acdbOpenObject(pPolyline,objId,AcDb::kForWrite);
				pPolyline = (AcDbPolyline *)pEntity;
				AcGePoint2d ptGe;

				ptGe[X]=ptCurrent[X];
				ptGe[Y] = ptCurrent[Y];
				pPolyline->addVertexAt(0,ptGe);
				pPolyline->close();
			}
			index++;
			acdbPointSet(ptCurrent,ptPrivious);
		}
		return eRet;
	}
public:

	// - ArxProject2.syj command (do not rename)
	static void ArxProject2syj(void)
	{
		Acad::ErrorStatus  eRet;
		//������ʵ��
		AcGePoint3d startPt(1.0,1.0,0.0);
		AcGePoint3d endPt(70.0,70.0,0.0);
		AcDbLine  *pLine = new AcDbLine(startPt,endPt);
		AcDbObjectId lineId;

		eRet = addEntityToModelSpace(pLine,lineId);
		if ( eRet != Acad::eOk)
		{
			return ;
		}
		// ����ص��������޷���ʾ
		pLine->close();

		//����һ��Բʵ��
		for (int i =0;i<10;i++)
		{
			ads_point pt;
			pt[0] = 5+i*10;
			pt[1] = 10+i*10;
			pt[2] = 0;
			drawCircle(pt);
			drawPoint(pt);
		}

	}

public:

	// - ArxProject2.syj_find command (do not rename)
	/*
	 *syj_find �������ݿ⺯��
	 */
	static void ArxProject2syj_find(void)
	{
		// Add your code for command ArxProject2.syj_find here
		//�����ݿⲢ�����������е�ʵ��.
		AcDbObjectId objId;
		AcDbDatabase * pDb = acdbHostApplicationServices()->workingDatabase();
		AcDbBlockTable *pBlockTable =NULL;
		pDb->getBlockTable(pBlockTable, AcDb::kForRead);
		AcDbBlockTableRecord *pBlockTableRecord =NULL;
		pBlockTable->getAt(ACDB_MODEL_SPACE,pBlockTableRecord, AcDb:: kForRead);
		pBlockTable->close();
		//������
		AcDbBlockTableRecordIterator *pIter = NULL;
		pBlockTableRecord->newIterator(pIter); // Create an iterator
		for (pIter->start(); !pIter->done(); pIter->step())
		{
			
			AcDbEntity *pEntity = NULL;
			if (pIter->getEntity(pEntity, AcDb::kForRead) != Acad::eOk)   
				continue;
			if(pEntity->isKindOf(AcDbCircle::desc())) 
				objId = pEntity->objectId();
		}
		pBlockTableRecord->close();
	}
public:

	// - ArxProject2.syj_line command (do not rename)
	
	/*
	 *syj_line ��һ���ߺͼ���Բ.
	 */
	static void ArxProject2syj_line(void)
	{
		ads_point pt1,pt2;
		acedGetPoint(NULL,_T("�����һ����:\n"),pt1);
		drawCircle(pt1);
		acedGetPoint(pt1,_T("����ڶ�����:\n"),pt2);
		drawCircle(pt2);
		drawLine(pt1,pt2);
	}
public:

	// - ArxProject2.syj_polyline command (do not rename)
	/*
	 *syj_polyline ������
	 */
	static void ArxProject2syj_polyline(void)
	{	
		drawPolyLine();
	}
public:

	// - ArxProject2.syj_arc command (do not rename)
	static void ArxProject2syj_arc(void)
	{
		// Add your code for command ArxProject2.syj_arc here
		drawArc();
	}
public:

	// - ArxProject2.syj_ss command (do not rename)
	static void ArxProject2syj_ss(void)
	{
		// Add your code for command ArxProject2.syj_ss here
		ads_name ssname;
		ads_point pt1,pt2;
		struct resbuf rb1,rb2,rb3,rb4,rb5;
		//rb1ɸѡ����ΪԲ��ʵ��
		rb1.restype = 0;
		ACHAR sbuff[10]=_T("CIRCLE");
		rb1.resval.rstring = sbuff;
		rb1.rbnext = NULL;
		//rb2ɸѡ����
		rb2.restype = 0;
		ACHAR sbuff1[20]=_T("LWPOLYLINE");
		rb2.resval.rstring = sbuff1;
		rb2.rbnext = NULL;
		//rb3ɸѡ��
		rb3.restype = 0;
		ACHAR sbuff2[10]=_T("LINE");
		rb3.resval.rstring = sbuff2;
		rb3.rbnext = NULL;

		int choices;
		acutPrintf(_T("��������:\n1.ѡ��Բ,2.ѡ�����,3ѡ����2"));
		acedGetInt(NULL,&choices);
		pt1[X] = pt1[Y] = pt1[Z] = 0.0;
		pt2[X] = pt2[Y] = 80.0;
		pt2[Z] = 0.0;
		switch(choices)
		{
		case 1:
			acedSSGet(NULL,NULL,NULL,&rb1,ssname);
			break;
		case 2:
			acedSSGet(NULL,NULL,NULL,&rb2,ssname);
			break;
		case 3:
			acedSSGet(NULL,NULL,NULL,&rb3,ssname);
			break;
		default:
			acutPrintf(_T("�������ִ���!!!!"));

		}
		ACHAR ss[128]={0};
		acedGetString(NULL,_T("����ѡ���ͼ������:\n"),ss);
		//ͼ��ѡ��
		rb4.restype = 8;
		rb4.resval.rstring = ss;
		rb4.rbnext = NULL;
		acedSSGet(NULL,NULL,NULL,&rb4,ssname);
		//
		rb5.restype = 62;
		rb5.resval.rint = 1;
		rb5.rbnext = NULL;
		acutPrintf(_T("ֻѡ���ɫʵ��"));
		acedSSGet(NULL,NULL,NULL,&rb5,ssname);
		//acedSSGet(_T("W"),pt1,pt2,&rb1,ssname);
		acedSSFree(ssname);
	}
public:

	// - ArxProject2.syj_ss2 command (do not rename)
	static void ArxProject2syj_ss2(void)
	{
		// Add your code for command ArxProject2.syj_ss2 here
		ads_name ss;
		resbuf rb;
		acedSSGet(NULL,NULL,NULL,NULL,ss);
		long len;
		//ȡ��ѡ�񼯵ĳ���
		acedSSLength(ss,&len);
		//����ѡ���е�ʵ��,����򿪲��޸�����ɫΪ��ɫ
		for (int i =0;i<len;i++)
		{
			ads_name entres;
			AcDbObjectId objId;
			AcDbEntity *entity = NULL;
			//ȡ��ѡ����ʵ�������ads_name
			acedSSName(ss,i,entres);
			//ȡ��ʵ���ObjectId
			acdbGetObjectId(objId,entres);
			//��ʵ��
			acdbOpenAcDbEntity(entity,objId,AcDb::kForWrite);
			//�޸�����ɫֵΪ1;
			entity->setColorIndex(1);
			entity->close();
		}
		acedSSFree(ss);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CArxProject2App)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj, syj, ACRX_CMD_TRANSPARENT, NULL)

ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_find, syj_find, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_line, syj_line, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_polyline, syj_polyline, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_arc, syj_arc, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_ss, syj_ss, ACRX_CMD_TRANSPARENT, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CArxProject2App, ArxProject2, syj_ss2, syj_ss2, ACRX_CMD_TRANSPARENT, NULL)

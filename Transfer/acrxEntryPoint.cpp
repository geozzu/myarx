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
#include "Transform.h"

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CTransferApp : public AcRxArxApp {

public:
	CTransferApp () : AcRxArxApp () {}

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

public:

	// - Transfer.syj_trans command (do not rename)
	static void Transfersyj_trans(void)
	{
		// Add your code for command Transfer.syj_trans here
		ads_name ss;
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
		CTransform tf(objIds);
		tf.read();
		tf.write();
		acedSSFree(ss);
	}
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CTransferApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CTransferApp, Transfer, syj_trans, syj_trans, ACRX_CMD_TRANSPARENT, NULL)

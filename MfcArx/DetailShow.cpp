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
//----- DetailShow.cpp : Implementation of CDetailShow
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include "DetailShow.h"

//-----------------------------------------------------------------------------
IMPLEMENT_DYNAMIC (CDetailShow, CAcUiDialog)

BEGIN_MESSAGE_MAP(CDetailShow, CAcUiDialog)
	ON_MESSAGE(WM_ACAD_KEEPFOCUS, OnAcadKeepFocus)
//	ON_LBN_SELCHANGE(IDC_LIST3, &CDetailShow::OnLbnSelchangeList3)
//ON_WM_INITMENU()
//ON_WM_CLOSE()
ON_WM_CLOSE()
ON_WM_DESTROY()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
CDetailShow::CDetailShow (AcDbObjectIdArray &objIds,CWnd *pParent /*=NULL*/, HINSTANCE hInstance /*=NULL*/) : CAcUiDialog (CDetailShow::IDD, pParent, hInstance),m_objIds(objIds)
{
}

//-----------------------------------------------------------------------------
void CDetailShow::DoDataExchange (CDataExchange *pDX) {
	CAcUiDialog::DoDataExchange (pDX) ;
	DDX_Control(pDX, IDC_LIST3, m_listBox);
}

//-----------------------------------------------------------------------------
//----- Needed for modeless dialogs to keep focus.
//----- Return FALSE to not keep the focus, return TRUE to keep the focus
LRESULT CDetailShow::OnAcadKeepFocus (WPARAM, LPARAM) {
	return (TRUE) ;
}
//void CDetailShow::OnInitMenu(CMenu* pMenu)
//{
//	CAcUiDialog::OnInitMenu(pMenu);
//
//	// TODO: 在此处添加消息处理程序代码
//	m_listBox.AddString(_T("aaaa"));
//}

//void CDetailShow::OnClose()
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//
//	CAcUiDialog::OnClose();
//}

BOOL CDetailShow::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	long len = m_objIds.length();
	//取得选择集的长度
	//遍历选择集中的实体,将其打开并修改其颜色为红色
	for (int i =0;i<len;i++)
	{
		ads_name entres;
		AcDbObjectId objId;
		AcDbEntity *obj;
		Acad::ErrorStatus es;
		es = acdbOpenAcDbEntity(obj, m_objIds[i], AcDb::kForRead, true);
		if (es == Acad::eOk) {
			LPCTSTR str;
			str = obj->isA()->name();
			m_listBox.AddString(str);
			obj->close();
		}
		
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDetailShow::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CAcUiDialog::OnClose();
	//this->DestroyWindow();
	//this->CloseWindow();
	//delete this;

}

void CDetailShow::OnDestroy()
{
	CAcUiDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

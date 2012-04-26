#include "StdAfx.h"
#include "Polyline.h"

CPolyline::CPolyline(AcDbObjectIdArray objIds):m_objIds(objIds)
{
}

CPolyline::~CPolyline(void)
{
}

void CPolyline::read(void)
{
	int len = m_objIds.length();
	for (int i = 0; i< len; i++)
	{
		AcDbEntity *pEnt = NULL;
		acdbOpenAcDbEntity(pEnt,m_objIds[i],AcDb::kForRead);
		pEnt->close();
		if (pEnt->isKindOf(AcDbPolyline::desc()))
		{
			AcDbPolyline *pPolyline = (AcDbPolyline *)pEnt;
			int numlen = pPolyline->numVerts();
			double bul;
			AcGeVector3d ve;
			//pPolyline->xData()
			for (int j=0;j<numlen;j++)
			{
				pPolyline->getBulgeAt(j,bul);
			}
			ve= pPolyline->normal();
			pPolyline->close();
		}
	}
}

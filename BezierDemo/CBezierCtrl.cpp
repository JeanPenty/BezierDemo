#include "stdafx.h"
#include "CBezierCtrl.h"


CBezierCtrl::CBezierCtrl()
{
	m_bDraw = false;
	m_bFirstDown = true;
}

CBezierCtrl::~CBezierCtrl()
{
	//
}

void CBezierCtrl::OnLButtonDown(UINT nFlags, SOUI::CPoint point)
{
	if (m_bFirstDown)
	{
		m_bDraw = true;
		m_bFirstDown = false;
		m_ptBezierStart = point;
	}
	else
	{
		m_bDraw = false;
		m_bFirstDown = true;
	}
}

void CBezierCtrl::OnLButtonUp(UINT nFlags, SOUI::CPoint point)
{
	//�ƶ���ɵڶ���������up
	if (m_bFirstDown)
	{
		m_bDraw = false;
		std::vector<Point> vecBezierPath;
		//��֯���ױ��������ߵ��ĸ���
		std::vector<CPoint> vecPts;
		//��һ����
		vecPts.push_back(m_ptBezierStart);
		//�ڶ�����
		CPoint pt2;
		pt2.x = m_ptBezierStart.x;
		pt2.y = point.y;
		vecPts.push_back(pt2);
		//��������
		CPoint pt3;
		pt3.x = point.x;
		pt3.y = m_ptBezierStart.y;
		vecPts.push_back(pt3);
		//���ĸ���
		vecPts.push_back(point);

		vector<Point> points;
		for (int i = 0; i < vecPts.size(); i++)
		{
			Point pt(vecPts[i].x, vecPts[i].y);
			points.push_back(pt);
		}
		if (vecPts.size() > 1)
		{
			//�������߹켣
			const float step = 0.02; // ����
			for (float t = 0; t <= 1; t += step)
			{
				Point p = bezier_curve(points, t);
				vecBezierPath.push_back(p);
			}
		}

		m_vecBezierPaths.push_back(vecBezierPath);
		Invalidate();
	}
}

void CBezierCtrl::OnMouseMove(UINT nFlags, SOUI::CPoint point)
{
	if (m_bDraw)
	{
		m_vecBezierPath.clear();

// 		//�ж�����֮���x��y��ֵ�������ֵС����ֵ����Ϊ���ױ��������ߣ���������Ϊ���ױ���������
// 		//��ֵ����Ϊ50
// 		if (std::abs(m_ptBezierStart.x - point.x) < 50 || std::abs(m_ptBezierStart.y - point.y) < 50)
// 		{
// 			//������Ϊ�����ε����˵㡢�׽�Ϊ30�ȵĵ���������ȥ��������㣻�������ζ���ȥ������ױ��������ߵĶ�λ��
// 			std::vector<CPoint> vecPts;
// 			//��һ����
// 			vecPts.push_back(m_ptBezierStart);
// 			//�ڶ�����
// 			CPoint pt;
// 			if (m_ptBezierStart.x == point.x)
// 				pt = m_ptBezierStart;
// 			else
// 				pt = getTriangleVertex(m_ptBezierStart, point);
// 			vecPts.push_back(pt);
// 			//��������
// 			vecPts.push_back(point);
// 
// 			vector<Point> points;
// 			for (int i = 0; i < vecPts.size(); i++)
// 			{
// 				Point pt(vecPts[i].x, vecPts[i].y);
// 				points.push_back(pt);
// 			}
// 			if (vecPts.size() > 1)
// 			{
// 				//�������߹켣
// 				const float step = 0.02; // ����
// 				for (float t = 0; t <= 1; t += step)
// 				{
// 					Point p = bezier_curve(points, t);
// 					m_vecBezierPath.push_back(p);
// 				}
// 			}
// 		}
// 		else
		{
			std::vector<CPoint> vecPts;
			//��һ����
			vecPts.push_back(m_ptBezierStart);
			//�ڶ�����
			CPoint pt2;
			pt2.x = m_ptBezierStart.x;
			pt2.y = point.y;
			vecPts.push_back(pt2);
			//��������
			CPoint pt3;
			pt3.x = point.x;
			pt3.y = m_ptBezierStart.y;
			vecPts.push_back(pt3);
			//���ĸ���
			vecPts.push_back(point);

			vector<Point> points;
			for (int i = 0; i < vecPts.size(); i++)
			{
				Point pt(vecPts[i].x, vecPts[i].y);
				points.push_back(pt);
			}
			if (vecPts.size() > 1)
			{
				//�������߹켣
				const float step = 0.02; // ����
				for (float t = 0; t <= 1; t += step)
				{
					Point p = bezier_curve(points, t);
					m_vecBezierPath.push_back(p);
				}
			}
		}
		Invalidate();
	}
}

LRESULT CBezierCtrl::OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	SetMsgHandled(FALSE);
	CPoint pt(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if ((uMsg == WM_MOUSEMOVE) && (wParam == MK_LBUTTON))
	{
		int iiii = 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
	}

	return 0;
}


void CBezierCtrl::OnPaint(IRenderTarget* pRT)
{
	pRT->SetAntiAlias(TRUE);

	CAutoRefPtr<IPen> pen, oldpen;
	pRT->CreatePen(PS_SOLID, RGBA(0, 0, 0, 255), 2, &pen);
	pRT->SelectObject(pen, (IRenderObj**)&oldpen);

	SAutoRefPtr<ICornerPathEffect> pathEffect;
	GETRENDERFACTORY->CreatePathEffect(__uuidof(ICornerPathEffect), (IPathEffect**)&pathEffect);
	if (pathEffect)
		pathEffect->Init(5.0);

	//������ʷ����
	for (int i = 0; i < m_vecBezierPaths.size(); i++)
	{
		std::vector<Point>& bezierPath = m_vecBezierPaths[i];
		CAutoRefPtr<IPath> path;
		GETRENDERFACTORY->CreatePath(&path);

		std::vector<POINT> vecPoly;
		for (int i = 0; i < bezierPath.size(); i++)
		{
			POINT pt;
			pt.x = bezierPath[i].x;
			pt.y = bezierPath[i].y;
			vecPoly.push_back(pt);
		}
		if (vecPoly.size() > 0)
			path->addPoly(&vecPoly[0], vecPoly.size(), false);

		pRT->DrawPath(path, pathEffect);
	}
	//��������ƶ�ʱ��������ʱ����
	if (m_bDraw)
	{
		CAutoRefPtr<IPath> path;
		GETRENDERFACTORY->CreatePath(&path);
		std::vector<POINT> vecPoly;
		for (int i = 0; i < m_vecBezierPath.size(); i++)
		{
			POINT pt;
			pt.x = m_vecBezierPath[i].x;
			pt.y = m_vecBezierPath[i].y;
			vecPoly.push_back(pt);
		}

		if (vecPoly.size() > 0)
			path->addPoly(&vecPoly[0], vecPoly.size(), false);

		pRT->DrawPath(path, pathEffect);
	}
	pRT->SelectObject(oldpen, NULL);

	SWindow::OnPaint(pRT);
}

// ���������
int CBezierCtrl::binomial(int n, int i) {
	int res = 1;
	for (int j = 1; j <= i; ++j) {
		res *= (n - j + 1) / (double)j;
	}
	return res;
}

// ����n�α����������ϵĵ�
CBezierCtrl::Point CBezierCtrl::bezier_curve(const vector<CBezierCtrl::Point>& points, float t) {
	int n = points.size() - 1;
	CBezierCtrl::Point res;
	for (int i = 0; i <= n; ++i) {
		float b = binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
		res.x = res.x + points[i].x * b;
		res.y = res.y + points[i].y * b;
	}
	return res;
}

void CBezierCtrl::RotatePoint(CPoint& pt, CPoint& ptBase, double dDegree)
{
	//���Ƕ�תΪ����
	double dRadians = M_PI * dDegree / 180.0;
	// �����������ת���ƶ���ԭ��
	pt.x -= ptBase.x;
	pt.y -= ptBase.y;

	// Ӧ����ת����
	double newX = pt.x * cos(dRadians) - pt.y * sin(dRadians);
	double newY = pt.x * sin(dRadians) + pt.y * cos(dRadians);

	// �����ƻ�ԭ����λ��
	pt.x = newX + ptBase.x;
	pt.y = newY + ptBase.y;
}

CPoint CBezierCtrl::getTriangleVertex(CPoint& pt1, CPoint& pt2)
{
	CPoint pt;
	//�����߶���б�Ƕ�
	//����б��
	double slope = (pt2.y - pt1.y) / (pt2.x - pt1.x);
	double angleRadians = atan(slope);
	double angleDegrees = angleRadians * 180.0 / M_PI;

	//��pt1Ϊ����˳ʱ����תpt2
	RotatePoint(pt2, pt1, angleDegrees);


	//double dRadians = M_PI * 30 / 180.0;  //�׽ǻ���
	double dis = sqrt(pow(pt2.x - pt1.x, 2) + pow(pt2.y - pt1.y, 2));
	double dH = 0.577 * dis / 2;

	if (pt1.x == pt2.x)  //��ֱ��
	{
		pt.x = pt1.x + dH;
		pt.y = pt1.y + std::abs(pt2.y - pt1.y) / 2;
	}
	else if (pt1.y == pt2.y)  //ˮƽ��
	{
		pt.x = pt1.x + std::abs(pt2.x - pt1.x) / 2;
		pt.y = pt1.y + dH;
	}

	//��pt1Ϊ������ʱ����תpt
	RotatePoint(pt, pt1, -angleDegrees);

	return pt;
}
#include "stdafx.h"
#include "CBezierCtrl.h"


CBezierCtrl::CBezierCtrl()
{
// 	vector<Point> points = { {100,100}, {200, 400},{400,400},{500,100}}; // 贝塞尔曲线控制点，给定控制点的数量决定贝塞尔曲线的阶数
// 
// 	const float step = 0.02; // 步长
// 	for (float t = 0; t <= 1; t += step)
// 	{
// 		Point p = bezier_curve(points, t);
// 		m_vecBezierPath.push_back(p);
// 	}

	m_bDraw = false;
}

CBezierCtrl::~CBezierCtrl()
{
	//
}

// void CBezierCtrl::OnLButtonDown(UINT nFlags, SOUI::CPoint point)
// {
// 	m_bDraw = true;
// }
// 
// void CBezierCtrl::OnLButtonUp(UINT nFlags, SOUI::CPoint point)
// {
// 	m_bDraw = false;
// 	m_vecBezierPath.clear();
// 	if (m_vecSrcPts.size() > 4)
// 	{
// 		return;
// 	}
// 	m_vecSrcPts.push_back(point);
// 
// 	vector<Point> points;
// 	for (int i = 0; i < m_vecSrcPts.size(); i++)
// 	{
// 		Point pt(m_vecSrcPts[i].x, m_vecSrcPts[i].y);
// 		points.push_back(pt);
// 	}
// 
// 	if (m_vecSrcPts.size() > 1)
// 	{
// 		const float step = 0.02; // 步长
// 		for (float t = 0; t <= 1; t += step)
// 		{
// 			Point p = bezier_curve(points, t);
// 			m_vecBezierPath.push_back(p);
// 		}
// 
// 		Invalidate();
// 	}
// 
// }
// 
// void CBezierCtrl::OnMouseMove(UINT nFlags, SOUI::CPoint point)
// {
// // 	if (m_bDraw)
// // 	{
// // 		m_vecSrcPts.push_back(point);
// // 		Invalidate();
// // 	}
// }

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
		//插入一个点
	}

	return 0;
}


void CBezierCtrl::OnPaint(IRenderTarget* pRT)
{
	pRT->SetAntiAlias(TRUE);

	CAutoRefPtr<IPen> pen, oldpen;
	pRT->CreatePen(PS_SOLID, RGBA(0, 0, 0, 255), 2, &pen);
	pRT->SelectObject(pen, (IRenderObj**)&oldpen);

	CAutoRefPtr<IPath> path;
	GETRENDERFACTORY->CreatePath(&path);

	SAutoRefPtr<ICornerPathEffect> pathEffect;
	GETRENDERFACTORY->CreatePathEffect(__uuidof(ICornerPathEffect), (IPathEffect**)&pathEffect);
	if (pathEffect)
		pathEffect->Init(5.0);

	std::vector<POINT> vecPoly;
	for (int i = 0; i < m_vecBezierPath.size(); i++)
	{
		POINT pt; 
		pt.x = m_vecBezierPath[i].x;
		pt.y = m_vecBezierPath[i].y;
		vecPoly.push_back(pt);
	}

	if (vecPoly.size() > 0)
	{
		path->addPoly(&vecPoly[0], vecPoly.size(), false);
	}
	
	pRT->DrawPath(path, pathEffect);
	//pRT->DrawLines(&vecPoly[0], vecPoly.size());

	pRT->SelectObject(oldpen, NULL);

	SWindow::OnPaint(pRT);
}

// 计算组合数
int CBezierCtrl::binomial(int n, int i) {
	int res = 1;
	for (int j = 1; j <= i; ++j) {
		res *= (n - j + 1) / (double)j; //(double)十分关键，不然j=i=n时，j为分数=0；
	}
	return res;
}

// 计算n次贝塞尔曲线上的点
CBezierCtrl::Point CBezierCtrl::bezier_curve(const vector<CBezierCtrl::Point>& points, float t) {
	int n = points.size() - 1;
	CBezierCtrl::Point res;
	for (int i = 0; i <= n; ++i) {
		//cout << "p:" << points[i].x << "," << points[i].y << endl;
		float b = binomial(n, i) * pow(t, i) * pow(1 - t, n - i);
		//cout << "bino=" << binomial(n, i) << endl;
		res.x = res.x + points[i].x * b;
		res.y = res.y + points[i].y * b;
		//res.z = res.z + points[i].z * b;
	}
	return res;
}
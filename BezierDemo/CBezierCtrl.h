#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>

using namespace std;

class CBezierCtrl : public SWindow
{
	SOUI_CLASS_NAME(CBezierCtrl, L"bezier")
public:
	CBezierCtrl();
	~CBezierCtrl();

	struct Point {
		float x, y;
		Point(float x = 0, float y = 0) :x(x), y(y) {}
	};

protected:
	void OnPaint(IRenderTarget* pRT);

// 	void OnLButtonDown(UINT nFlags, SOUI::CPoint point);
// 	void OnLButtonUp(UINT nFlags, SOUI::CPoint point);
// 	void OnMouseMove(UINT nFlags, SOUI::CPoint point);
	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
// 		MSG_WM_LBUTTONDOWN(OnLButtonDown)
// 		MSG_WM_LBUTTONUP(OnLButtonUp)
// 		MSG_WM_MOUSEMOVE(OnMouseMove)

		MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST, WM_MOUSELAST, OnMouseEvent)
		SOUI_MSG_MAP_END()

		SOUI_ATTRS_BEGIN()

		SOUI_ATTRS_END()

private:
	int binomial(int n, int i);
	Point bezier_curve(const vector<Point>& points, float t);


	std::vector<CPoint> m_vecSrcPts;
	std::vector<Point> m_vecBezierPath;


	bool m_bDraw;
public:

};
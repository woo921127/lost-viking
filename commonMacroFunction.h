#pragma once

//=====================================
// ## 18.11.20 ## ��ũ�θ� ��Ƶ��� ##
//=====================================

//����
#define PI 3.141592654f

inline POINTFLOAT PointFloatMake(float x, float y)
{
	POINTFLOAT pt = { x, y};
	return pt;
}
//��
typedef struct tagELLIPSE
{
	POINT p;
	float r;
} ELLIPSE, *LPELLIPSE;
inline void LineMake(HDC hdc, int startX, int startY, int endX, int endY)
{
	MoveToEx(hdc, startX, startY, NULL);
	LineTo(hdc, endX, endY);
}
//================= ��Ʈ ���� �Լ� ========================
//    ��Ʈ�� �����(����Ʈ, ž, ����ũ��, ����ũ��)
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x, y, x + width, y + height };
	return rc;
}
//    ��Ʈ�� �����(����X��ǥ, ����Y��ǥ, ����ũ��, ����ũ��)
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2) };
	return rc;
}
//================= �簢�� ���� �Լ� ======================
//          �簢�� �����Լ�(�ѷ���DC, left, top, ����ũ��, ����ũ��
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}
inline void RectangleMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}
inline void Rectangle(HDC hdc, RECT& rc)
{
	Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);
}
//====================== �� �� �� �� ���� ===============================
//        �� �����Լ� (hdc, left, top, ����ũ��, ����ũ��)
inline ELLIPSE EllipseMake(float x, float y, float r)
{
	ELLIPSE rt;
	rt.p.x = x;
	rt.p.y = y;
	rt.r = r;
	return rt;
}
//        �� �����Լ� (hdc, ������ǥ X, ������ǥ Y, ����ũ��, ����ũ��)
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - (width / 2), y - (height / 2), x + (width / 2), y + (height / 2));
}
//        �� �׸�
inline void EllipseRender(HDC hdc, ELLIPSE& e)
{
	Ellipse(hdc, e.p.x - e.r, e.p.y - e.r, e.p.x + e.r, e.p.y + e.r);
}

inline void Ellipse(HDC hdc, RECT& rc)
{
	Ellipse(hdc, rc.left, rc.top, rc.right, rc.bottom);
}

//���� ������ ��׸���
inline float RadToDeg(float angle)
{
	return angle * (180.0f / PI);
}
//��׸� ������ ��������
inline float DegToRad(float angle)
{
	return angle * (PI / 180.0f);
}

//�Ÿ����ϱ�
inline float GetDistance(POINT& p1, POINT& p2)
{
	return sqrtf((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
}
//���浹�ߴ���
inline bool EllipseCollision(ELLIPSE& e1, ELLIPSE& e2)
{
	return (e1.r + e2.r >= GetDistance(e1.p, e2.p));
}
//�� ������ ���̰�
inline float GetCenter(float a, float b)
{
	return a + (b - a) / 2;
}
//��Ʈ ���� ��ȯ�Լ�
inline POINT GetRectCenter(RECT& rc)
{
	POINT p;
	p.x = GetCenter(rc.left, rc.right);
	p.y = GetCenter(rc.top, rc.bottom);
	return p;
}
//��Ʈ x���� ��ȯ �Լ�
inline float GetRectXCenter(RECT& rc)
{
	return GetCenter(rc.left, rc.right);
}
//��Ʈ y���� ��ȯ �Լ�
inline float GetRectYCenter(RECT& rc)
{
	return GetCenter(rc.top, rc.bottom);
}
//��Ʈ width ��ȯ �Լ�
inline float GetRectWidth(RECT& rc)
{
	return rc.right - rc.left;
}
//��Ʈ height ��ȯ �Լ�
inline float GetRectHeight(RECT& rc)
{
	return rc.bottom - rc.top;
}
//�� �� �浹
inline bool PtInEllipse(ELLIPSE& e, POINT p)
{
	return (e.r >= GetDistance(e.p, p));
}
//�� �簢�� �浹
inline bool RectEllipseCollition(RECT& rc, ELLIPSE& e)
{
	if ((rc.left <= e.p.x && e.p.x <= rc.right) || (rc.top <= e.p.y && e.p.y <= rc.bottom))
	{
		RECT rc2 = { rc.left - e.r, rc.top - e.r, rc.right + e.r, rc.bottom + e.r };
		if (PtInRect(&rc2, e.p))return true;
	}
	else
	{
		POINT p;
		p.x = rc.left;
		p.y = rc.top;
		if (PtInEllipse(e, p)) return true;
		p.x = rc.left;
		p.y = rc.bottom;
		if (PtInEllipse(e, p)) return true;
		p.x = rc.right;
		p.y = rc.top;
		if (PtInEllipse(e, p)) return true;
		p.x = rc.right;
		p.y = rc.bottom;
		if (PtInEllipse(e, p)) return true;
	}
	return false;
}
//��Ʈ ��ĥ
inline void RectangleBrush(HDC hdc, RECT& rc, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	HBRUSH oBrush = (HBRUSH)SelectObject(hdc, brush);
	Rectangle(hdc, rc);
	SelectObject(hdc, oBrush);
	DeleteObject(brush);
}
//����ĥ
inline void EllipseBrush(HDC hdc, ELLIPSE& e, COLORREF color)
{
	HBRUSH brush = CreateSolidBrush(color);
	HBRUSH oBrush = (HBRUSH)SelectObject(hdc, brush);
	EllipseRender(hdc, e);
	SelectObject(hdc, oBrush);
	DeleteObject(brush);
}

//��׸� �ڻ���
inline float dCosf(float angle)
{
	return cosf(DegToRad(angle));
}
//��׸� ����
inline float dSinf(float angle)
{
	return sinf(DegToRad(angle));
}
#pragma once

#include "CCamObj.h"

template<typename T1, typename T2>
bool ISCollision(T1 _first, T2 _second)
{
	return false;
}

template<typename T1, typename T2>
int ALLRectCollision(T1 _first, T2 _second, OUT RECT& _rect)
{
	int iResult = 0;

	RECT src1 = RECT{};
	RECT src2 = RECT{};
	typename T1::iterator iter = _first.begin();
	for (; iter != _first.end(); ++iter)
	{
		src1 = RECT{ int((*iter)->GetPos().x - (*iter)->GetScale().left),int((*iter)->GetPos().y - (*iter)->GetScale().top),
			int((*iter)->GetPos().x + (*iter)->GetScale().right),int((*iter)->GetPos().y + (*iter)->GetScale().bottom) };
		typename T2::iterator iter2 = _second.begin();
		for (; iter2 != _second.end(); ++iter2)
		{
			src2 = RECT{ int((*iter2)->GetPos().x - (*iter2)->GetScale().left),int((*iter2)->GetPos().y - (*iter2)->GetScale().top),
			int((*iter2)->GetPos().x + (*iter2)->GetScale().right),int((*iter2)->GetPos().y + (*iter2)->GetScale().bottom) };
			if (IntersectRect(&_rect, &src1, &src2)) // �浹�� ���ٸ�
			{
				if (int(((CCamObj*)(*iter))->GetPrePos().x + (*iter)->GetScale().right) >= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().left) &&
					int(((CCamObj*)(*iter))->GetPrePos().x - (*iter)->GetScale().left) <= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().right)) // ���� ������ x�� �浹
				{
					if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().bottom) <= int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().top)) // ���� ������ ���� �ִ�
						iResult = COLL_UP;
					else if (int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) >= int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // ���� ������ �Ʒ��� �־���
						iResult = COLL_DOWN;
				}
				if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().bottom) > int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().top) &&
					int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) < int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // ���� ������ y�� �浹
				{
					// �����غ��� �̹� ���� �浹�� �����̴� ���� ���� ũ���ϰ� �񱳺��ٵ� ��ġ�ϰ� �񱳸� �ص� ū ������ ����. �ӵ� ����
					if (int(((CCamObj*)(*iter))->GetPrePos().x) <= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().left)) // ���� ������ ���ʿ� �ִ� 
						iResult = COLL_LEFT;
					else if (int(((CCamObj*)(*iter))->GetPrePos().x) >= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().right)) // ���� ������ �����ʿ� �ִ�
						iResult = COLL_RIGHT;
				}

				return iResult;
			}
		}
	}

	return iResult;
}

template<typename T1,typename T2>
void SAFE_DELETE_MAP(map<T1,T2> _mapPar)
{
	typename map<T1, T2>::iterator iter = _mapPar.begin();

	for (; iter != _mapPar.end(); ++iter)
	{
		delete iter->second;
	}
	_mapPar.clear();
}

float Lerf(float _fStart, float _fEnd,float _fDT);
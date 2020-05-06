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
			if (IntersectRect(&_rect, &src1, &src2)) // 충돌이 났다면
			{
				if (int(((CCamObj*)(*iter))->GetPrePos().x + (*iter)->GetScale().right) >= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().left) &&
					int(((CCamObj*)(*iter))->GetPrePos().x - (*iter)->GetScale().left) <= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().right)) // 이전 프레임 x축 충돌
				{
					if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().bottom) <= int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().top)) // 내가 이전에 위에 있다
						iResult = COLL_UP;
					else if (int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) >= int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // 내가 이전에 아래에 있었다
						iResult = COLL_DOWN;
				}
				if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().bottom) > int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().top) &&
					int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) < int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // 이전 프레임 y축 충돌
				{
					// 생각해보니 이미 나는 충돌한 상태이다 굳이 나의 크기하고 비교보다도 위치하고만 비교를 해도 큰 문제는 없다. 속도 때문
					if (int(((CCamObj*)(*iter))->GetPrePos().x) <= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().left)) // 내가 이전에 왼쪽에 있다 
						iResult = COLL_LEFT;
					else if (int(((CCamObj*)(*iter))->GetPrePos().x) >= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().right)) // 내가 이전에 오른쪽에 있다
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
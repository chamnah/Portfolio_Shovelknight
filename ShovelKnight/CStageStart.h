#pragma once
#include "CStage.h"
#include "CCamMgr.h"
#include "CTimeMgr.h"
#include "CTile.h"
class CDynamicObj;
class CStageStart :
	public CStage
{
private:
	HDC		m_hDC;
	HBITMAP m_hBit;

public:
	virtual int Progress();
	virtual void Enter();

private:
	template<typename T1, typename T2>
	void DynamicCollision(T1 first, T2 second);

public:
	CStageStart();
	virtual ~CStageStart();
};

// 충돌 처리 부분은 꼭 모듈화를 해보자
template<typename T1, typename T2>
inline void CStageStart::DynamicCollision(T1 first, T2 second)
{
	RECT result = RECT{};
	RECT src1 = RECT{};
	RECT src2 = RECT{};
	typename T1::iterator iter = first.begin();
	for (; iter != first.end(); ++iter)
	{
		src1 = RECT{ int((*iter)->GetPos().x - (*iter)->GetScale().x / 2.f),int((*iter)->GetPos().y - (*iter)->GetScale().y / 2.f),
			int((*iter)->GetPos().x + (*iter)->GetScale().x / 2.f),int((*iter)->GetPos().y + (*iter)->GetScale().y / 2.f) };
		typename T2::iterator iter2 = second.begin();
		for (; iter2 != second.end(); ++iter2)
		{
			if (((CTile*)(*iter2))->GetTileType() != TILE_TYPE::COLL)
				continue;
			src2 = RECT{ int((*iter2)->GetPos().x - (*iter2)->GetScale().x / 2.f),int((*iter2)->GetPos().y - (*iter2)->GetScale().y / 2.f),
			int((*iter2)->GetPos().x + (*iter2)->GetScale().x / 2.f),int((*iter2)->GetPos().y + (*iter2)->GetScale().y / 2.f) };
			if (IntersectRect(&result, &src1, &src2)) // 충돌이 났다면
			{
				if (int(((CCamObj*)(*iter))->GetPrePos().x + (*iter)->GetScale().x / 2.f) >= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().x / 2.f) &&
					int(((CCamObj*)(*iter))->GetPrePos().x - (*iter)->GetScale().x / 2.f) <= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().x / 2.f)) // 이전 프레임 x축 충돌
				{
					if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().y / 2.f) <= int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().y / 2.f)) // 내가 이전에 위에 있다
					{
						if (((CCamObj*)(*iter))->GetPrePos().y >= (*iter)->GetPos().y - (result.bottom - result.top)) // 만약 현재 좌표에서 뺀것보다 훨씬 멀리 보낸다면
						{
							((CCamObj*)(*iter))->SetPrePos((*iter)->GetPos().x, ((CCamObj*)(*iter))->GetPrePos().y);
							((CCamObj*)(*iter))->SetRealPos(((CCamObj*)(*iter))->GetRealPos().x, ((CCamObj*)(*iter))->GetPrePos().y); // 그게 아니라면
							(*iter)->SetPos((*iter)->GetPos().x, ((CCamObj*)(*iter))->GetPrePos().y);
						}
						else
						{
							((CCamObj*)(*iter))->SetPrePos((*iter)->GetPos().x, (*iter)->GetPos().y - (result.bottom - result.top));
							((CCamObj*)(*iter))->SetRealPos(((CCamObj*)(*iter))->GetRealPos().x, (*iter)->GetPos().y - (result.bottom - result.top)); // 그게 아니라면
							(*iter)->SetPos((*iter)->GetPos().x, (*iter)->GetPos().y - (result.bottom - result.top)); // 그게 아니라면
						}
						((CDynamicObj*)(*iter))->AccOn();
						//((CDynamicObj*)(*iter))->AccOff();
						((CDynamicObj*)(*iter))->SetJump(false);
					}
					//else if (int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) >= int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // 내가 이전에 아래에 있었다
					//{
					//}
				}
				if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().y / 2.f) > int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().y / 2.f) &&
					int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().y / 2.f) < int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().y / 2.f)) // 이전 프레임 y축 충돌
				{
					// 생각해보니 이미 나는 충돌한 상태이다 굳이 나의 크기하고 비교보다도 위치하고만 비교를 해도 큰 문제는 없다. 속도 때문
					if (int(((CCamObj*)(*iter))->GetPrePos().x) <= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().x / 2.f)) // 내가 이전에 왼쪽에 있다 
					{
						//CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetPreLook().x, CCamMgr::GetInst()->GetLook().y);
						//((CCamObj*)(*iter2))->SetPrePos(CCameraMgr::GetInst()->GetRealPos(((CCamObj*)(*iter2))->GetRealPos().x, ((CCamObj*)(*iter2))->GetRealPos().y));
						(*iter)->SetPos((*iter)->GetPos().x - (result.right - result.left), (*iter)->GetPos().y);// 플레이어 좌표 이동 나중에 몬스터에서는 요걸로 써야한다.
						//(*iter)->SetPos(((CCamObj*)(*iter))->GetPrePos().x, (*iter)->GetPos().y);
					}
					else if (int(((CCamObj*)(*iter))->GetPrePos().x) >= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().x / 2.f)) // 내가 이전에 오른쪽에 있다
					{
						//CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetPreLook().x, CCamMgr::GetInst()->GetLook().y);
						(*iter)->SetPos((*iter)->GetPos().x + (result.right - result.left), (*iter)->GetPos().y);
					}
				}
			}
		}
	}
}
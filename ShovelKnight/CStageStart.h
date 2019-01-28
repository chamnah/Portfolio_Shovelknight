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

// �浹 ó�� �κ��� �� ���ȭ�� �غ���
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
			if (IntersectRect(&result, &src1, &src2)) // �浹�� ���ٸ�
			{
				if (int(((CCamObj*)(*iter))->GetPrePos().x + (*iter)->GetScale().x / 2.f) >= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().x / 2.f) &&
					int(((CCamObj*)(*iter))->GetPrePos().x - (*iter)->GetScale().x / 2.f) <= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().x / 2.f)) // ���� ������ x�� �浹
				{
					if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().y / 2.f) <= int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().y / 2.f)) // ���� ������ ���� �ִ�
					{
						if (((CCamObj*)(*iter))->GetPrePos().y >= (*iter)->GetPos().y - (result.bottom - result.top)) // ���� ���� ��ǥ���� ���ͺ��� �ξ� �ָ� �����ٸ�
						{
							((CCamObj*)(*iter))->SetPrePos((*iter)->GetPos().x, ((CCamObj*)(*iter))->GetPrePos().y);
							((CCamObj*)(*iter))->SetRealPos(((CCamObj*)(*iter))->GetRealPos().x, ((CCamObj*)(*iter))->GetPrePos().y); // �װ� �ƴ϶��
							(*iter)->SetPos((*iter)->GetPos().x, ((CCamObj*)(*iter))->GetPrePos().y);
						}
						else
						{
							((CCamObj*)(*iter))->SetPrePos((*iter)->GetPos().x, (*iter)->GetPos().y - (result.bottom - result.top));
							((CCamObj*)(*iter))->SetRealPos(((CCamObj*)(*iter))->GetRealPos().x, (*iter)->GetPos().y - (result.bottom - result.top)); // �װ� �ƴ϶��
							(*iter)->SetPos((*iter)->GetPos().x, (*iter)->GetPos().y - (result.bottom - result.top)); // �װ� �ƴ϶��
						}
						((CDynamicObj*)(*iter))->AccOn();
						//((CDynamicObj*)(*iter))->AccOff();
						((CDynamicObj*)(*iter))->SetJump(false);
					}
					//else if (int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().top) >= int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().bottom)) // ���� ������ �Ʒ��� �־���
					//{
					//}
				}
				if (int(((CCamObj*)(*iter))->GetPrePos().y + (*iter)->GetScale().y / 2.f) > int(((CCamObj*)(*iter2))->GetPrePos().y - (*iter2)->GetScale().y / 2.f) &&
					int(((CCamObj*)(*iter))->GetPrePos().y - (*iter)->GetScale().y / 2.f) < int(((CCamObj*)(*iter2))->GetPrePos().y + (*iter2)->GetScale().y / 2.f)) // ���� ������ y�� �浹
				{
					// �����غ��� �̹� ���� �浹�� �����̴� ���� ���� ũ���ϰ� �񱳺��ٵ� ��ġ�ϰ� �񱳸� �ص� ū ������ ����. �ӵ� ����
					if (int(((CCamObj*)(*iter))->GetPrePos().x) <= int(((CCamObj*)(*iter2))->GetPrePos().x - (*iter2)->GetScale().x / 2.f)) // ���� ������ ���ʿ� �ִ� 
					{
						//CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetPreLook().x, CCamMgr::GetInst()->GetLook().y);
						//((CCamObj*)(*iter2))->SetPrePos(CCameraMgr::GetInst()->GetRealPos(((CCamObj*)(*iter2))->GetRealPos().x, ((CCamObj*)(*iter2))->GetRealPos().y));
						(*iter)->SetPos((*iter)->GetPos().x - (result.right - result.left), (*iter)->GetPos().y);// �÷��̾� ��ǥ �̵� ���߿� ���Ϳ����� ��ɷ� ����Ѵ�.
						//(*iter)->SetPos(((CCamObj*)(*iter))->GetPrePos().x, (*iter)->GetPos().y);
					}
					else if (int(((CCamObj*)(*iter))->GetPrePos().x) >= int(((CCamObj*)(*iter2))->GetPrePos().x + (*iter2)->GetScale().x / 2.f)) // ���� ������ �����ʿ� �ִ�
					{
						//CCamMgr::GetInst()->SetLook(CCamMgr::GetInst()->GetPreLook().x, CCamMgr::GetInst()->GetLook().y);
						(*iter)->SetPos((*iter)->GetPos().x + (result.right - result.left), (*iter)->GetPos().y);
					}
				}
			}
		}
	}
}
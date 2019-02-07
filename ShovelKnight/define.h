#pragma once

#define SINGLE(className) public: \
						static className* GetInst() \
						{\
							static className mgr;\
							return &mgr;\
						}\
						private:\
							className();\
							~className();

#define CLONE(className) \
						virtual className* GetClone()\
						{\
						    return new className(*this); \
						}

#define KEY(type, state) CKeyMgr::GetInst()->GetKeyState(type, state)
#define DT CTimeMgr::GetInst()->DeltaTime()
#define PI 3.14159265358979

#define COLL_LEFT 0x01
#define COLL_RIGHT 0x02
#define COLL_UP 0x04
#define COLL_DOWN 0x08

#define TILE_SIZE 64

#define TEX_LOAD (CTexture*)CResMgr::GetInst()->Load<CTexture*>
#define OBJ_TILE ((CTile*)m_vObj[(UINT)OBJ_TYPE::TILE][iIdx])
#define KEY_MGR CKeyMgr::GetInst()->GetKeyState

#define BOSS 0
#define ITEM 1
#define GOLD 2
#define LIFE 3

//#define LEFT  1
//#define RIGHT 2
//#define UP    3
//#define DOWN  4

#define HALF  1
#define EMPTY 0
#define FULL  2

#define JUMP_ONE 0x01
#define JUMP_END 0x02

#define CHANGE_STAGE INT_MAX - 1

enum class KEY_TYPE
{
	KEY_1 = 0,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_CTRL,
	KEY_ALT,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_TAB,
	KEY_CAP,
	KEY_ENTER,
	KEY_ESC,
	KEY_SPACE,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_NUM0,

	KEY_LBTN,
	KEY_RBTN,
	END,
};

enum class DIR
{
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum class KEY_STATE
{
	TAB,	// 최초 눌린 시점
	HOLD,   // 누르고 있는 중
	AWAY,   // 키를 막 뗀 시점
	NONE,   // 안눌림	
};

enum class STAGE
{
	LOGO,
	START,
	TOOL,
	ONE,
	TWO,
	END
};

enum class OBJ_TYPE
{
	BACK,
	TILE,
	OBJECT,
	MONSTER,
	PLAYER,
	DROP,
	SKILL,
	EFFECT,
	UI,
	STAGE_MOVE,
	END
};

enum class M_TYPE
{
	BEETO,
	END
};

enum class MOUSE_MODE
{
	NONE,
	TILE_PICK,
	TILE_COPY
};

enum class TILE_TYPE
{
	NONE,
	COLL,
	COPY
};

enum class UI_TYPE
{
	NONE,
	BUTTON,
	HP,
	VIEW,
	ARROW,
	COLL,
	LIST,
	INVEN,
	LAYER,
	END
};

enum class UI_CATEGORY
{
	TILE1,
	TILE2,
	OBJECT
};

enum class SKILL_TYPE
{
	BASIC,
	JUMP_ATTACK
};

enum class COIN_TYPE
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN
};

enum class ITEM_TYPE
{
	NONE,
	WAND
};
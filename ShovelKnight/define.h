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
#define OBJ CStageMgr::GetInst()->GetObjVector()

#define BOSS 0
#define ITEM 1
#define GOLD 2
#define LIFE 3

#define EMPTY 0
#define HALF  1
#define FULL  2

#define JUMP_ONE 0x01
#define JUMP_TWO 0x02
#define JUMP_THREE 0x04
#define JUMP_FOUR 0x08
#define JUMP_FIVE 0x10
#define JUMP_SIX 0x20
#define JUMP_END 0x40

#define CHANGE_STAGE INT_MAX - 1

#define LadderRiding 0x01
#define LadderColl   0x02
#define LadderExit   0x04

#define CHARGE_END 0x01
#define CHARGE_IDLE 0x02

#define SKIP 3

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
	DOWN,
	RIGHT_DOWN,
	LEFT_DOWN,
	RIGHT_UP,
	LEFT_UP
};

enum class EQUIP_TYPE
{
	BLUE,
	SILVER,
	YELLOW,
	PURPLE,
	BLACK,
	RED,
	SHINING,
	END
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
	THREE,
	FIVE,
	KING,
	M_ONE,
	M_TWO,
	M_THREE,
	M_FOUR,
	M_FIVE,
	M_SIX,
	M_SEVEN,
	M_EIGHT,
	M_NINE,
	ENDING,
	MAP,
	END
};

enum class OBJ_TYPE
{
	BACK,
	TILE,
	BLOCK,
	FIRE_BLOCK,
	MOVE_BLOCK,
	OBJECT,
	MONSTER,
	SHIELD,
	KING,
	HIDDEN,
	HIDDEN_TWO,
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
	BUBBLE_DRAGON,
	KING,
	SLIME,
	END
};

enum class MOUSE_MODE
{
	NONE,
	TILE_PICK,
	TILE_COPY,
	OBJ_PICK
};

enum class TILE_TYPE
{
	NONE,
	COLL,
	LADDER,
	COPY,
	NODE,
	TRAP,
	HIDDEN
};

enum class UI_TYPE
{
	NONE,
	BUTTON,
	HP,
	BOSS_HP,
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
	R_BASIC,
	L_BASIC,
	SMASH,
	JUMP_ATTACK,
	BUBBLE_ATTACK,
	DART_ATTACK,
	FIRE_BLOCK,
	LIGHTNIG
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

enum class BOSS_FSM
{
	WINDOW_VIEW,
	WINDOW_IDLE,
	BLOW,
	IDLE,
	FIGHT,
	RIGHT_ATTACK,
	LEFT_ATTACK,
	DOWN_ATTACK,
	UP_ATTACK,
	FIRE_ATTACK,
	END
};

enum class STAGE_COURSE
{
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	Eight,
	Nine,
	TEN
};
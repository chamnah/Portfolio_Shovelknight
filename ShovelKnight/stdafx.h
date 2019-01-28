// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#define _AFXDLL
#include <afxwin.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <vector>
using std::vector;

#include <map>
using std::map;
using std::make_pair;

#include <string>
using std::wstring;

#include <list>
using std::list;

#include <assert.h>

#pragma comment(lib,"msimg32")

#include "struct.h"
#include "define.h"
#include "function.h"
// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
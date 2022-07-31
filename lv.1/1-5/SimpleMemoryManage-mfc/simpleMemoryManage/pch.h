// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#define _CRT_SECURE_NO_WARNINGS
#define max_mem 100

#include <iostream>
#include <stdlib.h>
#include <vector>

#define F_FIT	0
#define B_FIT	1
#define	W_FIT	2

#define MAX_MEM 100

#endif //PCH_H

#ifndef _CHUNK

#define _CHUNK

typedef struct chunk {
	int mem_start = 0;
	int mem_left_size = 0;
	struct chunk* link = NULL;
} CHUNK;

#define SORT_CONDITION(mem1,mem2,tag) (((tag) == B_FIT) ? \
							((mem1)->mem_left_size > (mem2)->mem_left_size) : \
							((mem2)->mem_left_size > (mem1)->mem_left_size)
// 위의 전처리기는 다음 조건을 반환
// best fit일 경우 - (temp_1->mem_left_size > temp_2->mem_left_size)
// worst fit일 경우 - (temp_1->mem_left_size < temp_2->mem_left_size)

#endif

#ifndef _MEM_USAGE

#define _MEM_USAGE

typedef struct memoryUsage {
	int start = 0;
	int usage = 0;
}MEMUSE;

#endif

using namespace std;


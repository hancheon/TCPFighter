#include "stdafx.h"

void Asset(int condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo)
{
	if (condition) {
		return;
	}
	// TODO: 컨디션 확인하고 서버 다운 시키기
	// shutdownServer();

	std::format("! [error] Assert: {}, file: {}, line: {}", conditionStr, fileName, lineNo);
}

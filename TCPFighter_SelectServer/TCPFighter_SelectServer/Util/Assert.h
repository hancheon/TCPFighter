#pragma once

#undef ASSERT
#define ASSERT(x)	Assert(x, _W(#x), _W(__FILE__), __LINE__)

void Asset(int condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo);
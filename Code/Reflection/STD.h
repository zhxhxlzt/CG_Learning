#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <assert.h>
#include <iostream>


inline void Assert(bool success, std::string&& msg) {
	if (!success)
	{
		printf("%s", msg.data());
		assert(false);
	}
}
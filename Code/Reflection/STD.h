#pragma once
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <functional>
#include <assert.h>
#include <iostream>
#include <future>
#include <thread>
#include <queue>
#include <stack>

#define Move(x) std::move(x)

#define Print(x, ...) printf((x), ##__VA_ARGS__)

#define Assert(success, msg, ...)	if (!(success)) {Print(msg"\n", __VA_ARGS__); assert(success);}
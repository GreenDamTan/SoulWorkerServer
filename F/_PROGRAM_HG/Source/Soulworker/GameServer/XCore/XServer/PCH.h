// PCH.h - Precompiled header for GameServer
// This file ensures correct include order for Windows headers

#pragma once

// ============================================================================
// CRITICAL: Windows headers must be included in this specific order
// ============================================================================

#ifdef _WIN32
// Reduce Windows header includes to avoid conflicts
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Disable min/max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Include winsock2.h BEFORE windows.h
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif

// ============================================================================
// Standard library includes
// ============================================================================
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

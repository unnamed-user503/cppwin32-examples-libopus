# pragma once

#include <stdexcept>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define STRICT
#define STRICT_TYPED_ITEMIDS
#include <Windows.h>
#include <d2d1.h>
#include <d2d1_1.h>

#include <wil/result.h>
#include <wil/resource.h>
#include <wil/com.h>
#include <wil/win32_helpers.h>

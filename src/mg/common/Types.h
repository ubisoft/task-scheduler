#pragma once

#include "mg/common/Definitions.h"

#define F_DECLARE_CLASS(NAMESPACE_1, NAMESPACE_2, CLASS_TYPE) \
	namespace NAMESPACE_1 { namespace NAMESPACE_2 { class CLASS_TYPE; } }

#define F_DECLARE_CLASS_3(NAMESPACE_1, NAMESPACE_2, NAMESPACE_3, CLASS_TYPE) \
	namespace NAMESPACE_1 { namespace NAMESPACE_2 { namespace NAMESPACE_3 { class CLASS_TYPE; } } }

#define F_DECLARE_STRUCT(NAMESPACE_1, NAMESPACE_2, STRUCT_TYPE) \
	namespace NAMESPACE_1 { namespace NAMESPACE_2 { struct STRUCT_TYPE; } }

#define F_DECLARE_STRUCT_3(NAMESPACE_1, NAMESPACE_2, NAMESPACE_3, STRUCT_TYPE) \
	namespace NAMESPACE_1 { namespace NAMESPACE_2 { namespace NAMESPACE_3 { struct STRUCT_TYPE; } } }

#include <climits>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <errno.h>
#include <new>

#if IS_PLATFORM_WIN
#include <Windows.h>
#endif

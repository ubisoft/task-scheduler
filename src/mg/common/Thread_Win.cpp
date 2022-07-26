// Copyright (C) 2022 Ubisoft Entertainment
//
// This program is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 2 of
// the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <https://www.gnu.org/licenses/>.

#include "Thread.h"

#include <process.h>

namespace mg {
namespace common {

	void
	Thread::PrivSetName()
	{
		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType;
			LPCSTR szName;
			DWORD dwThreadID;
			DWORD dwFlags;
		} THREADNAME_INFO;

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = myName;
		info.dwThreadID = (DWORD)-1;
		info.dwFlags = 0;

		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}

	ThreadId
	GetCurrentThreadId()
	{
		return (ThreadId) ::GetCurrentThreadId();
	}

	void
	Sleep(
		uint32 aTimeMillis)
	{
		::Sleep(aTimeMillis);
	}

}
}

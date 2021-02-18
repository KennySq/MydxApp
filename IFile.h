#pragma once

namespace Mydx
{
	class IFile
	{
		virtual IFile* Load(const char* path) = 0;

		template<typename T>
		T* Cast() { return dynamic_cast<T*>(this); }
	};
}
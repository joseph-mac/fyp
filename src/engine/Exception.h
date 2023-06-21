#ifndef _MYENGINE_EXCEPTION_H_
#define _MYENGINE_EXCEPTION_H_

#include <exception>
#include <string>

namespace MYENGINE
{
	class Exception : public std::exception
	{
	public:
		Exception(const char* message)
		{
			m_message = message;
		}
		Exception(std::string message)
		{
			m_message = message.c_str();
		}
		
		~Exception() noexcept {}

		const char* what() { return m_message; }

	protected:
		const char* m_message;
	};
	
} // namespace MYENGINE

#endif

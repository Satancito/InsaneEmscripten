#pragma once
#ifndef INSANE_EXCEPTION_H
#define INSANE_EXCEPTION_H

#include <Insane/Insane.h>
#define USING_INSANE_EXCEPTION using namespace Insane::Exception

namespace Insane::Exception {

	class ExceptionBase : public std::exception {
	public:
		ExceptionBase(const String& _message = u8"Error", const int& code = 0);
		virtual std::string Message() const;
		virtual int Code() const;
	private:
		String message;
		int code;
	};

	class ParseException : public ExceptionBase {
	public:
		ParseException(const String& _message = u8"Parse exception.", const int& code = 0);
	private:
	};

	class CryptoException : public ExceptionBase {
	public:
		CryptoException(const String& _message = u8"Crypto exception.", const int& code = 0);
	private:
	};

	class ArgumentException : public ExceptionBase {
	public:
		ArgumentException(const String& _message = u8"Argument exception.", const int& code = 0);
	private:
	};
}

#endif //!INSANE_EXCEPTION_H
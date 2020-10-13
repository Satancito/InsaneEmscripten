#pragma once
#ifndef __INSANE_EXCEPTION_H__
#define __INSANE_EXCEPTION_H__

#include <Insane/Insane.h>
#define USING_INSANE_EXCEPTION using namespace Insane::Exception
namespace Insane::Exception {

	class ExceptionBase : public std::exception {
	public:
		ExceptionBase(const String& _message = "Error", const int& code = 0);
		virtual std::string Message() const;
		virtual int Code() const;
	private:
		String message;
		int code;
	};

	class ParseException : public ExceptionBase {
	public:
		ParseException(const String& _message = "Parse error.", const int& code = 0);
	private:
	};

	class CryptoException : public ExceptionBase {
	public:
		CryptoException(const String& _message = "Crypto error.", const int& code = 0);
	private:
	};
}

#endif //__INSANE_EXCEPTION_H__
#pragma once
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H
#include <Insane/InsaneString.h>
#define USING_INSANE_CRYPTO using namespace Insane::Crypto

#define LINE_BREAKS_APPEAR_MIME ((size_t)76)
#define LINE_BREAKS_APPEAR_PEM ((size_t)64)
#define NO_LINE_BREAKS ((size_t)0)

namespace Insane::Crypto
{
	enum class HashAlgorithm
	{
		Md5,
		Sha1,
		Sha256,
		Sha384,
		Sha512
	};

	class HashManager
	{
	public:
		[[nodiscard]] HashManager();
		~HashManager();
		[[nodiscard]] static String ToBase64(const String& data, size_t lineBreaks = NO_LINE_BREAKS, bool getUrlEncoded = false);
		[[nodiscard]] static String FromBase64(const String& base64);
		[[nodiscard]] static String InsertLineBreaks(const String& base64, size_t lineBreakAppear = LINE_BREAKS_APPEAR_MIME);
		[[nodiscard]] static String RemoveLineBreaks(const String& base64);
		[[nodiscard]] static String ToUrlEncodedBase64(const String& base64);
		[[nodiscard]] static String ToDefaultEncodedBase64(const String& base64);
		[[nodiscard]] static String ToRawHash(const String& data, HashAlgorithm algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToBase64Hash(const String& data, HashAlgorithm algorithm = HashAlgorithm::Sha512, size_t lineBreaks = NO_LINE_BREAKS, bool urlEncoded = false);
		[[nodiscard]] static String ToRawHmac(const String& data, const String& key, HashAlgorithm algorithm= HashAlgorithm::Sha512);
		[[nodiscard]] static String ToBase64Hmac(const String& data, const String& key, HashAlgorithm algorithm= HashAlgorithm::Sha512, size_t lineBreaks = NO_LINE_BREAKS, bool urlEncoded = false);
		[[nodiscard]] static String ToAlphanumericBase64(const String& data, size_t lineBreaks = NO_LINE_BREAKS);
	private:
	};

	class AesManager
	{
	public:
		[[nodiscard]] AesManager();
		~AesManager();
		[[nodiscard]] static String EncryptRaw(const String& data, const String& key) noexcept(false);
		[[nodiscard]] static String DecryptRaw(const String& data, const String& key) noexcept(false);
		[[nodiscard]] static String EncryptToBase64(const String& data, const String& key, bool urlEncoded = false) noexcept(false);
		[[nodiscard]] static String DecryptFromBase64(const String& data, const String& key) noexcept(false);
	private:
		[[nodiscard]] static String GenerateValidKey(const String& key);
	};

	class RandomManager
	{
	public:
		[[nodiscard]] RandomManager();
		~RandomManager();
		[[nodiscard]] static String Generate(int sz);
		[[nodiscard]] static int Generate(int min, int max);
		[[nodiscard]] static int Generate();
	private:
	};

	class RsaKeyPair {
	public:
		[[nodiscard]] RsaKeyPair();
		[[nodiscard]] RsaKeyPair(const std::string& publicKey, const std::string& privateKey);
		~RsaKeyPair();
		std::string GetPublicKey() const;
		std::string GetPrivateKey() const;
		void SetPublicKey(const std::string& publicKey);
		void SetPrivateKey(const std::string& privateKey);
		[[nodiscard]] String Serialize() const noexcept(false);
		[[nodiscard]] static RsaKeyPair Deserialize(String json) noexcept(false);
	private:
		std::string publicKey;
		std::string privateKey;
	};

	enum class RsaKeyEncoding
	{
		Ber,
		Pem,
		Xml,
		Json
	};

	class RsaManager {
	public:
		[[nodiscard]] RsaManager();
		~RsaManager();
		[[nodiscard]] static Insane::Crypto::RsaKeyPair CreateKeyPair(const Size& keySize = 4096, const RsaKeyEncoding &encoding = RsaKeyEncoding::Ber, const bool& indent = true);
		[[nodiscard]] static String EncryptRaw(const String& data, const String& publicKey, bool keyAsXml = false) noexcept(false);
		[[nodiscard]] static String DecryptRaw(const String& data, const String& privateKey, bool keyAsXml = false) noexcept(false);
		[[nodiscard]] static String EncryptToBase64(const String& data, const String& publicKey, bool keyAsXml = false, bool urlEncoded = false) noexcept(false);
		[[nodiscard]] static String DecryptFromBase64(const String& data, const String& privateKey, bool keyAsXml = false) noexcept(false);
	private:
		[[nodiscard]] static RsaKeyEncoding GetKeyEncoding(const String& key);
	};
}

#endif // !INSANE_CRYPOGRAPHY_H

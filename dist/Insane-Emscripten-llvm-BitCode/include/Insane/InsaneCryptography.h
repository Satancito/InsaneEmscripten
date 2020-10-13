#pragma once
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H
#include <Insane/InsaneString.h>
#define USING_INSANE_CRYPTO using namespace Insane::Crypto

#define AES_MAX_IV_LENGHT ((size_t)16)
#define AES_MAX_KEY_LENGTH ((size_t)32)

#define HMAC_INNER_PADDING ((Char)0x36)
#define HMAC_OUTER_PADDING ((Char)0x5c)
#define HMAC_64_BYTES_BLOCK_SIZE ((Int32)64)
#define HMAC_128_BYTES_BLOCK_SIZE ((Int32)128)

#define MD5_DIGEST_LENGTH 16
#define SHA1_DIGEST_LENGTH 20
#define SHA256_DIGEST_LENGTH 32
#define SHA384_DIGEST_LENGTH 48
#define SHA512_DIGEST_LENGTH 64

#define PEM_ENCODE_PRIVATE_KEY_HEADER_STR u8"-----BEGIN PRIVATE KEY-----"
#define PEM_ENCODE_PRIVATE_KEY_FOOTER_STR u8"-----END PRIVATE KEY-----"

#define PEM_ENCODE_PUBLIC_KEY_HEADER_STR u8"-----BEGIN PUBLIC KEY-----"
#define PEM_ENCODE_PUBLIC_KEY_FOOTER_STR u8"-----END PUBLIC KEY-----"

#define RSA_XML_KEY_MAIN_NODE_STR u8"RSAKeyValue"
#define RSA_XML_KEY_P_NODE_STR u8"P"
#define RSA_XML_KEY_Q_NODE_STR u8"Q"
#define RSA_XML_KEY_DP_NODE_STR u8"DP"
#define RSA_XML_KEY_DQ_NODE_STR u8"DQ"
#define RSA_XML_KEY_INVERSEQ_NODE_STR u8"InverseQ"
#define RSA_XML_KEY_D_NODE_STR u8"D"
#define RSA_XML_KEY_MODULUS_NODE_STR u8"Modulus"
#define RSA_XML_KEY_EXPONENT_NODE_STR u8"Exponent"

#define RSA_PADDING_ALGORITHM u8"EME-PKCS1-v1_5"

#define LINE_BREAKS_APPEAR_MIME ((size_t)76)
#define LINE_BREAKS_APPEAR_PEM ((size_t)64)
#define NO_LINE_BREAKS ((size_t)0)

namespace Insane::Crypto
{
	enum class HashAlgorithm
	{
		MD5,
		SHA1,
		SHA256,
		SHA384,
		SHA512
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
		[[nodiscard]] static String ToRawHash(const String& data, HashAlgorithm algorithm);
		[[nodiscard]] static String ToBase64Hash(const String& data, HashAlgorithm algorithm, size_t lineBreaks = NO_LINE_BREAKS, bool urlEncoded = false);
		[[nodiscard]] static String ToRawHmac(const String& data, const String& key, HashAlgorithm algorithm);
		[[nodiscard]] static String ToBase64Hmac(const String& data, const String& key, HashAlgorithm algorithm, size_t lineBreaks = false, bool urlEncoded = false);
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

	class RsaManager {
	public:
		[[nodiscard]] RsaManager();
		~RsaManager();
		[[nodiscard]] static Insane::Crypto::RsaKeyPair CreateKeyPair(const SizeT& keySize = 4096, bool keyAsXml = false, bool indentXml = false) noexcept(false);
		[[nodiscard]] static String EncryptRaw(const String& data, const String& publicKey, bool keyAsXml = false) noexcept(false);
		[[nodiscard]] static String DecryptRaw(const String& data, const String& privateKey, bool keyAsXml = false) noexcept(false);
		[[nodiscard]] static String EncryptToBase64(const String& data, const String& publicKey, bool keyAsXml = false, bool urlEncoded = false) noexcept(false);
		[[nodiscard]] static String DecryptFromBase64(const String& data, const String& privateKey, bool keyAsXml = false) noexcept(false);
	private:
	};


}

#endif // !INSANE_CRYPOGRAPHY_H

#pragma once
#ifndef INSANE_CRYPOGRAPHY_H
#define INSANE_CRYPOGRAPHY_H

#include <Insane/InsaneString.h>

#define USING_INSANE_CRYPTO using namespace Insane::Crypto

#define MIME_LINE_BREAKS_LENGTH ((size_t)76)
#define PEM_LINE_BREAKS_LENGTH ((size_t)64)
#define NO_LINE_BREAKS ((size_t)0)
#define HMAC_KEY_SIZE ((size_t)(16))
#define SCRYPT_SALT_SIZE ((size_t)16)
#define SCRYPT_ITERATIONS ((size_t)(32768))
#define SCRYPT_BLOCKSIZE ((size_t)(8))
#define SCRYPT_PARALLELISM ((size_t)(1))
#define SCRYPT_DERIVED_KEY_LENGTH ((size_t)(64))
#define ARGON2_DERIVED_KEY_LENGTH ((size_t)(64))
#define ARGON2_SALT_SIZE ((size_t)16)
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

	enum class Argon2Variant
	{
		Argon2d,
		Argon2i,
		Argon2id
	};

	class HmacResult
	{
	public:
		[[nodiscard]] HmacResult(const std::string &hash, const std::string &key, const HashAlgorithm algorithm);
		~HmacResult() = default;
		String Hash() const;
		String Key() const;
		HashAlgorithm Algorithm() const;
		String RawHash() const;
		String RawKey() const;
		String Serialize() const;
		static HmacResult Deserialize(const String &json);

	private:
		[[nodiscard]] HmacResult();
		String hash;
		String key;
		HashAlgorithm algorithm;
	};

	class ScryptResult
	{
	public:
		[[nodiscard]] ScryptResult(const std::string &hash, const std::string &salt, const size_t &iterations, const size_t &blockSize, const size_t &parallelism, const size_t &derivedKeyLength);
		~ScryptResult() = default;
		String Hash() const;
		String Salt() const;
		size_t Iterations() const;
		size_t BlockSize() const;
		size_t Parallelism() const;
		size_t DerivedKeyLength() const;
		String RawHash() const;
		String RawSalt() const;
		String Serialize() const;
		static ScryptResult Deserialize(const String &json);

	private:
		[[nodiscard]] ScryptResult();
		String hash;
		String salt;
		size_t iterations;
		size_t blockSize;
		size_t parallelism;
		size_t derivedKeyLength;
	};

	class Argon2Result
	{
	public:
		[[nodiscard]] Argon2Result(const std::string &hash, const std::string &salt, const Argon2Variant &variant, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const size_t &derivedKeyLength);
		String Hash() const;
		String Salt() const;
		Argon2Variant Variant() const;
		size_t Iterations() const;
		size_t MemorySizeKiB() const;
		size_t Parallelism() const;
		size_t DerivedKeyLength() const;
		String RawHash() const;
		String RawSalt() const;

		~Argon2Result() = default;
		String Serialize() const;
		static Argon2Result Deserialize(const String &json);

	private:
		[[nodiscard]] Argon2Result();
		String hash;
		String salt;
		Argon2Variant variant;
		size_t iterations;
		size_t memorySizeKiB;
		size_t parallelism;
		size_t derivedKeyLength;
	};

	class RandomManager
	{
	public:
		~RandomManager();
		[[nodiscard]] static String Next(size_t sz);
		[[nodiscard]] static int Next(int min, int max);
		[[nodiscard]] static int Next();

	private:
		[[nodiscard]] RandomManager() = default;
	};

	class HashManager
	{
	public:
		~HashManager() = default;
		[[nodiscard]] static String InsertLineBreaks(const String &data, size_t lineBreakAppear = MIME_LINE_BREAKS_LENGTH);
		[[nodiscard]] static String RemoveLineBreaks(const String &data);
		[[nodiscard]] static String ToBase64(const String &data, size_t lineBreaksLength = NO_LINE_BREAKS, const bool &removePadding = false);
		[[nodiscard]] static String FromBase64(const String &data);
		[[nodiscard]] static String ToUrlSafeBase64(const String &data);
		[[nodiscard]] static String ToFilenameSafeBase64(const String &data);
		[[nodiscard]] static String ToUrlEncodedBase64(const String &data);
		[[nodiscard]] static String ToAlphanumericBase64(const String &data, size_t lineBreaksLength = NO_LINE_BREAKS);
		[[nodiscard]] static String ToRawHash(const String &data, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToBase64Hash(const String &data, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToRawHmac(const String &data, const String &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static HmacResult ToBase64Hmac(const String &data, const String &key, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static HmacResult ToBase64Hmac(const String &data, const size_t &keySize = HMAC_KEY_SIZE, const HashAlgorithm &algorithm = HashAlgorithm::Sha512);
		[[nodiscard]] static String ToRawScrypt(const String &data, const String &salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
		[[nodiscard]] static ScryptResult ToBase64Scrypt(const String &data, const size_t &saltSize = SCRYPT_SALT_SIZE, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
		[[nodiscard]] static ScryptResult ToBase64Scrypt(const String &data, const String &salt, const bool &isBase64Salt, const size_t &iterations = SCRYPT_ITERATIONS, const size_t &blockSize = SCRYPT_BLOCKSIZE, const size_t &parallelism = SCRYPT_PARALLELISM, const size_t &derivedKeyLength = SCRYPT_DERIVED_KEY_LENGTH);
		[[nodiscard]] static String ToRawArgon2(const String &data, const String &salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
		[[nodiscard]] static Argon2Result ToBase64Argon2(const String &data, size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const size_t saltSize = ARGON2_SALT_SIZE, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
        [[nodiscard]] static Argon2Result ToBase64Argon2(const String &data, const String &salt, const bool& isBase64Salt, const size_t &iterations, const size_t &memorySizeKiB, const size_t &parallelism, const Argon2Variant &variant = Argon2Variant::Argon2id, const size_t &derivedKeyLength = ARGON2_DERIVED_KEY_LENGTH);
	private:
		[[nodiscard]] HashManager() = default;
	};

	class AesManager
	{
	public:
		~AesManager();
		[[nodiscard]] static String EncryptRaw(const String &data, const String &key) noexcept(false);
		[[nodiscard]] static String DecryptRaw(const String &data, const String &key) noexcept(false);
		[[nodiscard]] static String EncryptToBase64(const String &data, const String &key) noexcept(false);
		[[nodiscard]] static String DecryptFromBase64(const String &data, const String &key) noexcept(false);

	private:
		[[nodiscard]] AesManager() = default;
		[[nodiscard]] static String GenerateValidKey(const String &key);
	};

	class RsaKeyPair
	{
	public:
		[[nodiscard]] RsaKeyPair(const std::string &publicKey, const std::string &privateKey);
		std::string PublicKey() const;
		std::string PrivateKey() const;
		void SetPublicKey(const std::string &publicKey);
		void SetPrivateKey(const std::string &privateKey);
		[[nodiscard]] String Serialize() const noexcept(false);
		[[nodiscard]] static RsaKeyPair Deserialize(String json) noexcept(false);
		~RsaKeyPair() = default;

	private:
		[[nodiscard]] RsaKeyPair() = default;
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

	class RsaManager
	{
	public:
		~RsaManager() = default;
		[[nodiscard]] static Insane::Crypto::RsaKeyPair CreateKeyPair(const Size &keySize = 4096, const RsaKeyEncoding &encoding = RsaKeyEncoding::Ber, const bool &indent = true);
		[[nodiscard]] static String EncryptRaw(const String &data, const String &publicKey) noexcept(false);
		[[nodiscard]] static String DecryptRaw(const String &data, const String &privateKey) noexcept(false);
		[[nodiscard]] static String EncryptToBase64(const String &data, const String &publicKey) noexcept(false);
		[[nodiscard]] static String DecryptFromBase64(const String &data, const String &privateKey) noexcept(false);
		[[nodiscard]] static String ConvertKey(const String &key, const RsaKeyEncoding &encoding);
		// [[nodiscard]] static bool IsValidPublicKey(const String& publicKey);
		// [[nodiscard]] static bool IsValidPrivateKey(const String& privateKey);
	private:
		[[nodiscard]] RsaManager() = default;
	};
} // namespace Insane::Crypto

#endif // !INSANE_CRYPOGRAPHY_H

/* TODO
 Converter de claves
 */
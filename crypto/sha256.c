#include "hblk_crypto.h"

/**
 * sha256 - computes the hash of a sequence of bytes
 * @s: sequence of bytes to be hashed (8bit byte array)
 * @len: length of s
 * @digest: storage for hash
 *
 * Return: pointer to digest or NULL
 */

uint8_t *sha256(int8_t const *s, size_t len, uint8_t digest[SHA256_DIGEST_LENGTH])
{
	if (!s || !len || !digest)
		return (NULL);

	SHA256((unsigned char *)s, len, (unsigned char *)digest);

	return (digest);
}

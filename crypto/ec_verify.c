#include "hblk_crypto.h"
#include "string.h"

/**
 * ec_verify - verifies a signature of a message with a key
 * @key: the EC_KEY struct
 * @msg: the message (data) to encrypt
 * @msglen: the length of msg
 * @sig: the buffer to hold the signature
 *
 * Return: 0 or 1
 */

int ec_verify(EC_KEY const *key,
	      uint8_t const *msg,
	      size_t msglen,
	      sig_t const *sig)
{
	if (!key || !msg || !sig || sig->len < 1)
		return (0);

	if (!ECDSA_verify(0,
			  msg,
			  msglen,
			  sig->sig,
			  (int)sig->len,
			  (EC_KEY *)key))
		return (0);

	return (1);
}

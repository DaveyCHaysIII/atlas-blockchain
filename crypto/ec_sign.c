#include "hblk_crypto.h"
#include "string.h"

/**
 * ec_sign - creates a signature of a message with a key
 * @key: the EC_KEY struct
 * @msg: the message (data) to encrypt
 * @msglen: the length of msg
 * @sig: the buffer to hold the signature
 *
 * Return: pointer to sig->sig or NULL
 */

uint8_t *ec_sign(EC_KEY const *key,
		 uint8_t const *msg,
		 size_t msglen,
		 sig_t *sig)
{
	if (!key || !msg)
		return (NULL);

	if (!ECDSA_sign(0,
			msg,
			msglen,
			sig->sig,
			(unsigned int *)&sig->len,
			(EC_KEY *)key))
		return (NULL);

	return (sig->sig);
}

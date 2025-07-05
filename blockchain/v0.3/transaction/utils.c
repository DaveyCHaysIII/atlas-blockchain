#include "transactions.h"

/**
 * match_pub - check if pub key matches priv key
 * @priv: the private key
 * @pub: the public key
 *
 * Return: 1 on found, otherwise 0
 */

int match_pub(EC_KEY const *priv, uint8_t pub[EC_PUB_LEN]);
{
	uint8_t derived_pub[EC_PUB_LEN];

	if(!ec_to_pub(priv, derived_pub))
		return (0);

	if (memcmp(derived_pub, pub, EC_PUB_LEN) == 0)
		return (1);

	return (0);
}

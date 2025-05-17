#include "hblk_crypto.h"

/**
 * ec_create - creates a new pair of keys
 * note: we do not have access to what the EC_KEY struct looks like
 *
 * Return: pointer to EC_KEY struct or NULL
 */

EC_KEY *ec_create(void)
{
	EC_KEY *key;

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	if (!key)
	{
		return (NULL);
	}
	if (!EC_KEY_generate_key(key))
	{
		return (NULL);
	}
	return (key);
}

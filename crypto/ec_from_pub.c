#include "hblk_crypto.h"

/**
 * ec_from_pub - create an EC_KEY from a public key
 * @pub: the octal public key
 *
 * Return: pointer to EC_KEY or NULL
 */

EC_KEY *ec_from_pub(uint8_t const pub[EC_PUB_LEN])
{
	EC_POINT *point;
	EC_KEY *key;
	const EC_GROUP *group;

	if (!pub)
		return (NULL);

	key = EC_KEY_new_by_curve_name(EC_CURVE);
	group = EC_KEY_get0_group(key);
	point = EC_POINT_new(group);

	if (!EC_POINT_oct2point(group, point, pub, EC_PUB_LEN, NULL))
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}

	if (!EC_KEY_set_public_key(key, point))
	{
		EC_POINT_free(point);
		EC_KEY_free(key);
		return (NULL);
	}
	EC_POINT_free(point);
	return (key);
}

#include "hblk_crypto.h"

/**
 * ec_to_pub - extracts a public key from an opaque struct
 * @key: pointer to EC_KEY
 * @pub: the address to store the public key
 *
 * Return: pointer to pub or NULL
 */

uint8_t *ec_to_pub(EC_KEY const *key, uint8_t pub[EC_PUB_LEN])
{
	if (!key || !pub)
		return (NULL);

	const EC_GROUP *group;
	const EC_POINT *point;

	group = EC_KEY_get0_group(key);
	point = EC_KEY_get0_public_key(key);

	if (!group || !point)
		return (NULL);

	if (!EC_POINT_point2oct(group,
				point,
				POINT_CONVERSION_UNCOMPRESSED,
				pub,
				EC_PUB_LEN,
				NULL))
		return (NULL);

	return (pub);
}

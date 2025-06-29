#include "transaction.h"

/**
 * tx_out_create - creates and initializes a new tx_out_t
 * @amount: the amount of coin it holds
 * @pub: the wallet that holds it
 *
 * Return: pointer to newly created tx_out_t
 */

tx_out_t *tx_out_create(uint32_t amount, uint8_t const pub[EC_PUB_LEN])
{
	tx_out_t *new_out;

	if (!amount || !pub)
		return (NULL);

	new_out = calloc(1, sizeof(tx_out_t));
	if (!new_out)
		return (NULL);

	new_out->amount = amount;
	memcpy(new_out->pub, pub, EC_PUB_LEN);
	SHA256((unsigned char *)new_out,
	       (sizeof(new_out->amount) + sizeof(new_out->pub)),
	       new_out->hash);

	return (new_out);
}

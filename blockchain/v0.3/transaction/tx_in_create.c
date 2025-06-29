#include "transaction.h"

/**
 * tx_in_create - creates a new tx_input (inputs spend outputs)
 * @unspent: the unspent_tx_out_t we're splitting
 *
 * Return: pointer to new tx_in_t or NULL
 */

tx_in_t *tx_in_create(unspent_tx_out_t const *unspent)
{
	tx_in_t *new_in;

	if (!unspent)
		return (NULL);

	new_in = calloc(1, sizeof(tx_in_t));

	memcpy(new_in->block_hash, unspent->block_hash, SHA256_DIGEST_LENGTH);
	memcpy(new_in->tx_id, unspent->tx_id, SHA256_DIGEST_LENGTH);
	memcpy(new_in->tx_out_hash, unspent->out.hash, SHA256_DIGEST_LENGTH);

	return (new_in);
}

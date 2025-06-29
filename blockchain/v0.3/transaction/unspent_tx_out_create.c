#include "transaction.h"

/**
 * unspent_tx_out_create - creates and initializes a new unspent_tx_out_t
 * @block_hash: the hash of the block this references
 * @tx_id: the id of the transaction_t this references
 * @out: the actual output we are referencing
 *
 * Return: pointer to new unspent_tx_out_t or NULL
 */

unspent_tx_out_t *unspent_tx_out_create(
	uint8_t block_hash[SHA256_DIGEST_LENGTH],
	uint8_t tx_id[SHA256_DIGEST_LENGTH],
	tx_out_t const *out)
{

	unspent_tx_out_t *new_u_out;

	if (!block_hash || !tx_id || !out)
		return (NULL);

	new_u_out = calloc(1, sizeof(unspent_tx_out_t));
	if (!new_u_out)
		return (NULL);

	memcpy(new_u_out->block_hash, block_hash, SHA256_DIGEST_LENGTH);
	memcpy(new_u_out->tx_id, tx_id, SHA256_DIGEST_LENGTH);
	new_u_out->out = *out;
	return (new_u_out);
}


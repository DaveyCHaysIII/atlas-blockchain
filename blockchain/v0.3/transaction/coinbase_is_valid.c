#include "transaction.h"

/**
 * coinbase_is_valid - Checks if coinbase is valid
 * @coinbase: coinbase in queston
 * @block_index: index of block mined
 *
 * Return: 1 or 0
 */
int coinbase_is_valid(transaction_t const *coinbase,
		      uint32_t block_index)
{
	static const uint8_t zero_32[32];
	static const uint8_t zero_72[72];
	uint8_t hash[SHA256_DIGEST_LENGTH];
	tx_in_t *in;
	tx_out_t *out;

	if (!coinbase)
		return (0);
	if (!block_index)
		block_index = 0;

	transaction_hash(coinbase, hash);
	if (memcmp(coinbase->id, hash, SHA256_DIGEST_LENGTH))
		return (0);

	if (llist_size(coinbase->inputs) != 1)
		return (0);
	if (llist_size(coinbase->outputs) != 1)
		return (0);

	in = llist_get_tail(coinbase->inputs);
	if (memcmp(in->tx_out_hash, &block_index, sizeof(uint32_t)))
		return (0);
	if (memcmp(in->block_hash, zero_32, 32) != 0)
		return (0);
	if (memcmp(in->sig.sig, zero_72, 72) != 0)
		return (0);

	out = llist_get_tail(coinbase->outputs);
	if (out->amount != COINBASE_AMOUNT)
		return (0);
	return (1);
}

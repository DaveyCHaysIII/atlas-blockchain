#include "transaction.h"

/**
 * coinbase_create - Creates a coinbase transaction
 * @receiver: key struct of reciever
 * @block_index: index of block mined
 *
 * Return: new transaction pointer or NULL
 */
transaction_t *coinbase_create(EC_KEY const *receiver,
			       uint32_t block_index)
{
	transaction_t *tx;
	tx_out_t *out;
	tx_in_t *in;
	uint8_t pub[EC_PUB_LEN];

	if (!receiver)
		return (NULL);
	if (!block_index)
		block_index = 0;

	tx = calloc(1, sizeof(transaction_t));
	if (!tx)
		return (NULL);

	ec_to_pub(receiver, pub);
	out = tx_out_create(COINBASE_AMOUNT, pub);
	in = calloc(1, sizeof(tx_in_t));
	if (!in)
	{
		free(tx);
		free(out);
		return (NULL);
	}
	memcpy(in->tx_out_hash, &block_index, sizeof(uint32_t));

	tx->inputs = llist_create(MT_SUPPORT_FALSE);
	tx->outputs = llist_create(MT_SUPPORT_FALSE);

	llist_add_node(tx->inputs, in, ADD_NODE_REAR);
	llist_add_node(tx->outputs, out, ADD_NODE_REAR);
	transaction_hash(tx, tx->id);

	return (tx);
}

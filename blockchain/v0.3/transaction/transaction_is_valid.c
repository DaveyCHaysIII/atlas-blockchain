#include "transaction.h"

/**
 * match_tx_out_hash - matches hash
 * @node: the node in the list
 * @hash: the hash to check match
 *
 * Return: 0 or 1
 */
static int match_tx_out_hash(llist_node_t node, void *hash)
{
	return (memcmp(((unspent_tx_out_t *)node)->out.hash,
		hash,
		SHA256_DIGEST_LENGTH) == 0);
}

/**
 * validate_input_sig - validates input signatures
 * @in: the input in question
 * @tx: the transaction
 * @all_unspent: pointer to the unspent list
 * @in_total: the in total accumulator
 *
 * Return: 0 or 1
 */
static int validate_input_sig(tx_in_t const *in,
			      transaction_t const *tx,
			      llist_t *all_unspent,
			      uint32_t *in_total)
{
	unspent_tx_out_t *utxo;
	EC_KEY *pub_key = NULL;
	int valid;

	utxo = llist_find_node(all_unspent,
			       match_tx_out_hash,
			       (void *)(in->tx_out_hash));
	if (!utxo)
		return (0);
	pub_key = ec_from_pub(utxo->out.pub);
	if (!pub_key)
		return (0);

	valid = ec_verify(pub_key, tx->id, SHA256_DIGEST_LENGTH, &in->sig);
	EC_KEY_free(pub_key);

	if (valid)
		*in_total += utxo->out.amount;
	return (valid);
}

/**
 * transaction_is_valid - checks if a transaction is valid
 * @transaction: the transaction in question
 * @all_unspent: the unspent list
 *
 * Return: 0 or 1
 */

int transaction_is_valid(transaction_t const *transaction,
			 llist_t *all_unspent)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];
	uint32_t total_input = 0, total_output = 0;
	tx_in_t *in;
	tx_out_t *out;
	int i, n_in, n_out;

	if (!transaction || !all_unspent)
		return (0);
	if (!transaction_hash(transaction, hash) ||
	    memcmp(hash, transaction->id, SHA256_DIGEST_LENGTH) != 0)
		return (0);
	n_in = llist_size(transaction->inputs);
	for (i = 0; i < n_in; i++)
	{
		in = llist_get_node_at(transaction->inputs, i);
		if (!in)
			return (0);
		if (!validate_input_sig(in, transaction,
					all_unspent, &total_input))
			return (0);
	}
	n_out = llist_size(transaction->outputs);
	for (i = 0; i < n_out; i++)
	{
		out = llist_get_node_at(transaction->outputs, i);
		if (!out)
			return (0);
		total_output += out->amount;
	}
	return (total_input == total_output);
}

#include "transaction.h"

/**
 * transaction_hash - create a hash of a transaction
 * @transaction: the transaction in question
 * @hash_buf: the place to store the transaction hash
 *
 * Return: pointer to hash_buf
 */

uint8_t *transaction_hash(
	transaction_t const *transaction,
	uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	size_t len_in, len_out, total_size, i, counter;
	uint8_t *hash_buf_temp;
	tx_in_t *current_in;
	tx_out_t *current_out;


	len_in = llist_size(transaction->inputs);
	len_out = llist_size(transaction->outputs);

	total_size = (96 * len_in) + (64 * len_out);
	hash_buf_temp = calloc(1, total_size);

	counter = 0;
	for (i = 0; i < len_in; i++)
	{
		current_in = (tx_in_t *)llist_get_node_at(
				transaction->inputs,
				i);
		memcpy(hash_buf_temp + counter, current_in, 96);
		counter += 96;
	}
	for (i = 0; i < len_out; i++)
	{
		current_out = (tx_out_t *)llist_get_node_at(
				transaction->outputs,
				i);
		memcpy(hash_buf_temp + counter, current_out, 64);
		counter += 64;
		len_out--;
	}
	SHA256(hash_buf_temp, total_size, hash_buf);
	return (hash_buf);
}

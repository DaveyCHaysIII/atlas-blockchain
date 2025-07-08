#include "blockchain.h"

static int grab_ids(llist_node_t trans, unsigned int i, void *buffer);

/**
 * block_hash - function that hashes an entire block
 * @block: the block to be hashed
 * @hash_buf: the buffer to save the hash into
 *
 * Return: pointer to hash_buff or NULL
 */

uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH])
{
	uint8_t *buffer;
	uint32_t total_size, trans_len, block_mem;

	if (!block || !hash_buf)
		return (NULL);

	trans_len = llist_size(block->transactions);
	block_mem = sizeof(block->info) + block->data.len;
	total_size = block_mem + (trans_len * SHA256_DIGEST_LENGTH);

	buffer = calloc(1, total_size);
	memcpy(buffer, block, block_mem);

	llist_for_each(block->transactions, grab_ids, buffer + block_mem);

	SHA256(buffer, total_size, hash_buf);
	free(buffer);
	return (hash_buf);
}


/**
 * grab_ids - grabs the id of transaction and puts it in buffer
 * @trans: the transaction in question
 * @i: the index of the transaction
 * @buffer: the buffer to write to
 *
 * Return: 0 on success
 */

static int grab_ids(llist_node_t trans, unsigned int i, void *buffer)
{
	memcpy((uint8_t *)buffer + i * SHA256_DIGEST_LENGTH,
	       ((transaction_t *)trans)->id, SHA256_DIGEST_LENGTH);
	return (0);

}

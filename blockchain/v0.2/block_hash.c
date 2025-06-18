#include "blockchain.h"

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
	if (!block || !hash_buf)
		return (NULL);

	SHA256((unsigned char *)block,
	       sizeof(block->info) + block->data.len,
	       hash_buf);
	return (hash_buf);

}

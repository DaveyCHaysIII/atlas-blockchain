#include "blockchain.h"

/**
 * block_create - creates a block
 * @prev: a pointer to the previous block
 * @data: the data to init in the new block
 * @data_len: the length of the data we're using
 *
 * Return: pointer to the new block
 */

block_t *block_create(block_t const *prev,
		      int8_t const *data,
		      uint32_t data_len)
{
	block_t *new_block;

	if (!prev || !data || !data_len)
		return (NULL);

	new_block = calloc(1, sizeof(block_t));
	if (!new_block)
		return (NULL);

	new_block->info.index = (prev->info.index) + 1;
	new_block->info.difficulty = 0;
	new_block->info.timestamp = time(NULL);
	new_block->info.nonce = 0;
	memcpy(new_block->info.prev_hash,
	       prev->hash,
	       SHA256_DIGEST_LENGTH);
	memcpy(new_block->data.buffer,
	       data,
	       data_len > BLOCKCHAIN_DATA_MAX ? BLOCKCHAIN_DATA_MAX : data_len);
	new_block->data.len = data_len;

	return (new_block);
}

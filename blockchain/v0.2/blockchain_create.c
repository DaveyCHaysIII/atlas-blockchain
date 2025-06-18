#include "blockchain.h"

/**
 * blockchain_create - creates the blockchain object
 *
 * Return: pointer to a blockchain object
 */

blockchain_t *blockchain_create(void)
{
	blockchain_t *new_chain;
	block_t *genesis;

	new_chain = calloc(1, sizeof(blockchain_t));
	if (!new_chain)
	{
		return (NULL);
	}
	new_chain->chain = NULL;
	genesis = calloc(1, sizeof(block_t));
	if (!genesis)
	{
		return (NULL);
	}
	new_chain->chain = llist_create(MT_SUPPORT_FALSE);
	if (!new_chain->chain)
	{
		free(new_chain);
		free(genesis);
		return (NULL);
	}
	genesis->info.index = 0;
	genesis->info.difficulty = 0;
	genesis->info.timestamp = GEN_TIMESTAMP;
	genesis->info.nonce = 0;
	memset(genesis->info.prev_hash, 0, SHA256_DIGEST_LENGTH);
	memcpy(genesis->hash, HOLBERTON_HASH, SHA256_DIGEST_LENGTH);
	genesis->data.len = 16;
	memcpy(genesis->data.buffer, DATA, genesis->data.len);
	if (llist_add_node(new_chain->chain, genesis, ADD_NODE_REAR) == -1)
	{
		llist_destroy(new_chain->chain, 0, NULL);
		free(new_chain);
		free(genesis);
		return (NULL);
	}
	return (new_chain);
}

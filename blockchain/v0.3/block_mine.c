#include "blockchain.h"

#define EXP 24
#define MAX_NONCE (1UL << EXP)

/**
 * block_mine - "mines" a block, creates a hash for a block
 * @block: the block to mine
 *
 * Return: no return
 */

void block_mine(block_t *block)
{
	uint8_t hash[SHA256_DIGEST_LENGTH];

	if (!block)
		return;

	for (block->info.nonce = 0;
	     block->info.nonce < MAX_NONCE;
	     block->info.nonce++)
	{
		block_hash(block, hash);
		if (hash_matches_difficulty(hash, block->info.difficulty))
		{
			memcpy(block->hash, hash, SHA256_DIGEST_LENGTH);
			return;
		}
	}
	fprintf(stderr, "HIT MAX NONCE\n");
}

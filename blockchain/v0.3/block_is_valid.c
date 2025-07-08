#include "blockchain.h"


int trans_valid(transaction_t *trans, unsigned int i, llist_t *unspent);
int check_hash(block_t const *block, uint8_t *buffer);
int check_genesis(block_t const *block);

/**
 * block_is_valid - checks to see if the current block is a valid blockchain
 * @block: pointer to the block to check
 * @prev_block: pointer to the previous block
 * @all_unspent: pointer to the unspent list
 *
 * Return: 0 or -1
 */

int block_is_valid(block_t const *block, block_t const *prev_block,
		   llist_t *all_unspent)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
	uint8_t cmp_hash[SHA256_DIGEST_LENGTH];
	transaction_t *trans;

	if (!block)
		return (-1);
	if (!prev_block && block->info.index != 0)
		return (-1);
	if (block->info.index == 0)
	{
		if (check_genesis(block) != 0)
			return (-1);
		return (0);
	}
	else if (block->info.index == 1)
	{
		if (check_genesis(prev_block) != 0)
			return (-1);
	}
	if (block->info.index != prev_block->info.index + 1)
		return (-1);
	if (check_hash(prev_block, prev_hash) != 0 ||
	    check_hash(block, cmp_hash) != 0)
		return (-1);
	if (memcmp(prev_block->hash,
		   block->info.prev_hash,
		   SHA256_DIGEST_LENGTH) != 0)
		return (-1);
	trans = (transaction_t *)llist_get_head(block->transactions);
	if (!coinbase_is_valid(trans, block->info.index))
		return (-1);
	if (llist_for_each(block->transactions,
			   (node_func_t)&trans_valid,
			   all_unspent))
		return (-1);
	return (0);
}

/**
 * check_genesis - checks if a block is a genesis block
 * @block: the block to check
 *
 * Return: 0 on success
 */

int check_genesis(block_t const *block)
{
	block_t genesis = {
		.info = {0, 0, 1537578000, 0, {0}},
		.data = {"Holberton School", 16},
		.hash = HOLBERTON_HASH,
		.transactions = NULL
		};

	return (memcmp(block, &genesis, sizeof(block_t)));
}

/**
 * check_hash - checks if a block hash matches
 * @block: the block to check its hash
 * @buffer: a uint8_t buffer of length SHA256_DIGEST_LENGTH
 *
 * Return: 0 on success, -1 on fail
 */

int check_hash(block_t const *block, uint8_t *buffer)
{
	block_hash(block, buffer);
	if (!hash_matches_difficulty(block->hash, block->info.difficulty))
		return (-1);
	if (memcmp(block->hash, buffer, SHA256_DIGEST_LENGTH) != 0)
		return (-1);
	return (0);
}

/**
 * trans_valid - checks if all transactions are valid
 * @trans: transaction to validate
 * @i: index of transaction
 * @unspent: pointer to the unspent list
 *
 * Return: 0 or 1
 */

int trans_valid(transaction_t *trans, unsigned int i, llist_t *unspent)
{
	if (i == 0)
		return (0);
	if (!transaction_is_valid(trans, unspent))
		return (1);
	return (0);
}

#include "blockchain.h"


void print_hash(uint8_t const *hash);
int check_hash(block_t const *block, uint8_t *buffer);
int check_genesis(block_t const *block);

/**
 * block_is_valid - checks to see if the current block is a valid blockchain
 * @block: pointer to the block to check
 * @prev_block: pointer to the previous block
 *
 * Return: 0 or -1
 */

int block_is_valid(block_t const *block, block_t const *prev_block)
{
	uint8_t prev_hash[SHA256_DIGEST_LENGTH];
	uint8_t cmp_hash[SHA256_DIGEST_LENGTH];

	if (!block)
		return (-1);
	if (!prev_block && block->info.index != 0)
		return (-1);
	if (block->info.index == 0)
	{
		if (check_genesis(block) != 0)
		{
			printf("block isn't genesis!\n");
			return (-1);
		}
		return (0);
	}
	else if (block->info.index == 1)
	{
		if (check_genesis(prev_block) != 0)
		{
			fprintf(stderr, "prev block isn't genesis!\n");
			return (-1);
		}
	}
	if (block->info.index != prev_block->info.index + 1)
	{
		fprintf(stderr, "Indecies are incorrect\n");
		return (-1);
	}
	if (check_hash(prev_block, prev_hash) != 0 ||
	    check_hash(block, cmp_hash) != 0)
		return (-1);
	if (memcmp(prev_block->hash,
		   block->info.prev_hash,
		   SHA256_DIGEST_LENGTH) != 0)
		return (-1);
	return (0);
}

/**
 * check_genesis - checks if a block is a genesis block
 * @block: the block to check
 *
 * Return: 1 on success, 0 on fail
 */

int check_genesis(block_t const *block)
{
	block_t genesis = {
		{0, 0, 1537578000, 0, {0}},
		{"Holberton School", 16},
		HOLBERTON_HASH};

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
	if (memcmp(block->hash, buffer, SHA256_DIGEST_LENGTH) != 0)
	{
		fprintf(stderr, "hash's don't match\n");
		print_hash(block->hash);
		print_hash(buffer);
		return (-1);
	}
	return (0);
}

/**
 * print_hash - helper function for check_hash
 * @hash: the hash buffer to print
 *
 * Return: no return
 */
void print_hash(uint8_t const *hash)
{
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
		fprintf(stderr, "%02x", hash[i]);
	fprintf(stderr, "\n");
}

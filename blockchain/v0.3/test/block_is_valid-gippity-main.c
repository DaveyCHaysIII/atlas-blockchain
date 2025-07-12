#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blockchain.h"

void _print_hex_buffer(uint8_t const *buf, size_t len);

/**
 * add_block - creates, hashes, and validates a new block
 * @bc: the blockchain
 * @prev: the previous block
 * @data: string to store in new block's data field
 * @miner: the EC key to use for coinbase creation
 *
 * Return: pointer to the new block or NULL on failure
 */
block_t *add_block(blockchain_t *bc, block_t const *prev, char const *data, EC_KEY *miner)
{
	block_t *block;
	transaction_t *coinbase;
	int tx_count;

	block = block_create(prev, (int8_t const *)data, strlen(data));
	if (!block)
	{
		fprintf(stderr, "Failed to create block with data: %s\n", data);
		return (NULL);
	}
	printf("Size of block->transactions at creation: %d\n", llist_size(block->transactions));

	coinbase = coinbase_create(miner, block->info.index);
	if (!coinbase)
	{
		fprintf(stderr, "Failed to create coinbase for index %u\n", block->info.index);
		free(block);
		return (NULL);
	}

	if (llist_add_node(block->transactions, coinbase, ADD_NODE_REAR) != 0)
	{
		fprintf(stderr, "Failed to add coinbase to block\n");
		free(coinbase);
		free(block);
		return (NULL);
	}
	printf("Size of block->transactions after coinbase add: %d\n", llist_size(block->transactions));

	/* Debug: Check transaction count */
	tx_count = llist_size(block->transactions);
	if (tx_count != 1)
		fprintf(stderr, "WARNING: Block has %d transactions before mining!\n", tx_count);

	block_mine(block);

	/* Print info before validation */
	printf("Block %u mined with hash: ", block->info.index);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n");

	printf("Size of block->transactions after mine, right before valid call: %d\n", llist_size(block->transactions));


	if (block_is_valid(block, prev, bc->unspent) != 0)
	{
		fprintf(stderr, "Block %u is INVALID!\n", block->info.index);
		block_destroy(block);
		return (NULL);
	}

	if (llist_add_node(bc->chain, block, ADD_NODE_REAR) != 0)
	{
		fprintf(stderr, "Failed to add block %u to chain\n", block->info.index);
		block_destroy(block);
		return (NULL);
	}

	return (block);
}


/**
 * main - entry point for block_is_valid test
 */
int main(void)
{
	blockchain_t *blockchain;
	block_t *block;
	EC_KEY *miner;

	miner = ec_create();
	if (!miner)
	{
		fprintf(stderr, "Failed to create EC key\n");
		return (EXIT_FAILURE);
	}

	blockchain = blockchain_create();
	if (!blockchain)
	{
		fprintf(stderr, "Failed to create blockchain\n");
		EC_KEY_free(miner);
		return (EXIT_FAILURE);
	}

	block = llist_get_head(blockchain->chain);
	printf("Genesis block at index %u with hash: ", block->info.index);
	_print_hex_buffer(block->hash, SHA256_DIGEST_LENGTH);
	printf("\n");

	/* Add blocks and validate */
	block = add_block(blockchain, block, "Block 1 data", miner);
	if (!block)
		goto cleanup;

	block = add_block(blockchain, block, "Block 2 data", miner);
	if (!block)
		goto cleanup;

	block = add_block(blockchain, block, "Block 3 data", miner);
	if (!block)
		goto cleanup;

	printf("All blocks added and validated successfully!\n");

cleanup:
	blockchain_destroy(blockchain);
	EC_KEY_free(miner);
	return (EXIT_SUCCESS);
}

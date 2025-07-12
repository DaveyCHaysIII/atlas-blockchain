#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blockchain.h"

void print_block_info(block_t const *block);

/**
 * main - Test block_create function
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
	block_t genesis = {
		.info = {0, 0, 1537578000, 0, {0}},
		.data = {"Genesis Block", 13},
		.hash = HOLBERTON_HASH,
		.transactions = NULL
	};

	const char *data = "Block 1 data!";
	block_t *new_block = block_create(&genesis, (int8_t *)data, strlen(data));

	if (!new_block)
	{
		fprintf(stderr, "block_create returned NULL\n");
		return (EXIT_FAILURE);
	}

	print_block_info(new_block);

	/* Cleanup */
	llist_destroy(new_block->transactions, 0, NULL);
	free(new_block);
	return (EXIT_SUCCESS);
}

/**
 * print_block_info - Print key info about a block
 * @block: block to print
 */
void print_block_info(block_t const *block)
{
	if (!block)
		return;

	printf("Index: %u\n", block->info.index);
	printf("Difficulty: %u\n", block->info.difficulty);
	printf("Timestamp: %lu\n", block->info.timestamp);
	printf("Nonce: %lu\n", block->info.nonce);
	printf("Prev Hash (first 4 bytes): %02x %02x %02x %02x ...\n",
	       block->info.prev_hash[0], block->info.prev_hash[1],
	       block->info.prev_hash[2], block->info.prev_hash[3]);
	printf("Data length: %u\n", block->data.len);
	printf("Data: %.*s\n", block->data.len, block->data.buffer);
	printf("Transactions list: %s\n", block->transactions ? "Initialized" : "NULL");
}

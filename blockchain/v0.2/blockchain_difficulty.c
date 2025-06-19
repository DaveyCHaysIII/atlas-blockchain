#include "blockchain.h"

/**
 * blockchain_difficulty - finds  difficulty of a blockchain
 * @blockchain: the blockchain in question
 *
 * Return: difficulty of next potential block
 */

uint32_t blockchain_difficulty(blockchain_t const *blockchain)
{
	block_t *current, *prev_adjust;
	int size, prev_adjust_index;
	uint64_t expected, actual;

	if (!blockchain)
		return (0);

	current = llist_get_tail(blockchain->chain);
	size = llist_size(blockchain->chain);
	if (size > DIFFICULTY_ADJUSTMENT_INTERVAL)
		prev_adjust_index = size - DIFFICULTY_ADJUSTMENT_INTERVAL;
	else
		prev_adjust_index = 0;
	fprintf(stderr,
		"Current Index: %d\nprev_adjust_index: %d\n",
		size,
		prev_adjust_index);
	if (current->info.index % DIFFICULTY_ADJUSTMENT_INTERVAL == 0 &&
	    current->info.index != 0)
	{
		fprintf(stderr, "Made it, index: %d\n", size);
		prev_adjust = llist_get_node_at(blockchain->chain,
						prev_adjust_index);
		expected = (current->info.index - prev_adjust->info.index) *
			    BLOCK_GENERATION_INTERVAL;
		actual = current->info.timestamp - prev_adjust->info.timestamp;
		fprintf(stderr, "Expected: %ld\nActual: %ld\n", expected, actual);
		if (actual > expected * 2)
			return (current->info.difficulty - 1);
		if (actual < (expected / 2))
			return (current->info.difficulty + 1);
		else
			return (current->info.difficulty);
	}
	return (current->info.difficulty);
}

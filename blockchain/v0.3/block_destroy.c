#include "blockchain.h"

/**
 * block_destroy - destorys a block
 * @block: the block to destroy
 *
 * Return: no return
 */

void block_destroy(block_t *block)
{
	if (!block)
		return;
	llist_destroy(block->transactions,
		      1,
		      transaction_destroy);
	free(block);
}


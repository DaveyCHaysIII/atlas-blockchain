#include "blockchain.h"

/**
 * block_destroy - destorys a block
 * @block: the block to destroy
 *
 * Return: no return
 */

void block_destroy(block_t *block)
{
	if (block)
	{
		if (llist_size(block->transactions) > 0)
			llist_destroy(block->transactions, 1, NULL);
		else
			llist_destroy(block->transactions, 0, NULL);
		free(block);
	}
}


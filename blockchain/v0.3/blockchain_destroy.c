#include "blockchain.h"

/**
 * blockchain_destroy - destroys a blockchain
 * @blockchain: the blockchain_t in question
 *
 * Return: no return
 */

void blockchain_destroy(blockchain_t *blockchain)
{
	if (!blockchain)
		return;

	llist_destroy(blockchain->chain, 1, NULL);
	llist_destroy(blockchain->unspent, 1, NULL);
	free(blockchain);
}

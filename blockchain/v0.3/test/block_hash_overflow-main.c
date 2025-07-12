#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "blockchain.h"

int main(void)
{
	block_t *genesis, *block;
	blockchain_t *chain;
	transaction_t *coinbase, *tx;
	EC_KEY *sender, *receiver;
	llist_t *all_unspent;
	tx_out_t *out;
	uint8_t pub[EC_PUB_LEN];

	/* Set up */
	chain = blockchain_create();
	genesis = llist_get_head(chain->chain);

	sender = ec_create();
	receiver = ec_create();
	all_unspent = llist_create(MT_SUPPORT_FALSE);

	/* Make a block and add coinbase */
	block = block_create(genesis, (int8_t *)"Test block", 10);
	coinbase = coinbase_create(sender, block->info.index);
	llist_add_node(block->transactions, coinbase, ADD_NODE_REAR);

	/* Add fake UTXO to list */
	out = tx_out_create(100, ec_to_pub(sender, pub));
	unspent_tx_out_t *utxo = unspent_tx_out_create(
		genesis->hash, coinbase->id, out);
	llist_add_node(all_unspent, utxo, ADD_NODE_REAR);

	/* Add a real transaction */
	tx = transaction_create(sender, receiver, 100, all_unspent);
	if (!tx)
	{
		fprintf(stderr, "Failed to create transaction\n");
		return EXIT_FAILURE;
	}
	llist_add_node(block->transactions, tx, ADD_NODE_REAR);

	/* 🔥 This is the call we want to test */
	block_hash(block, block->hash);

	printf("Hash computed:\n");
	for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		printf("%02x", block->hash[i]);
	printf("\n");

	/* Cleanup */
	block_destroy(block);
	blockchain_destroy(chain);
	EC_KEY_free(sender);
	EC_KEY_free(receiver);
	llist_destroy(all_unspent, 1, free);
	free(out);

	return EXIT_SUCCESS;
}

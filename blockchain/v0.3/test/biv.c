#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "blockchain.h"

// Helper to print a hex buffer (SHA256 hashes)
void print_hash(uint8_t *hash)
{
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        printf("%02x", hash[i]);
}

// Add a block with given data and miner key, returns the new block or NULL
block_t *add_block(blockchain_t *bc, block_t *prev, const char *data, EC_KEY *miner)
{
    printf("Adding block after index %u with data '%s'\n", prev ? prev->info.index : 0, data);

    block_t *block = block_create(prev, (int8_t *)data, (uint32_t)strlen(data));
    if (!block)
    {
        fprintf(stderr, "Failed to create block\n");
        return NULL;
    }

    block->info.difficulty = 4; // Low difficulty for fast mining in tests

    transaction_t *coinbase = coinbase_create(miner, block->info.index);
    if (!coinbase)
    {
        fprintf(stderr, "Failed to create coinbase transaction\n");
        block_destroy(block);
        return NULL;
    }
    llist_add_node(block->transactions, coinbase, ADD_NODE_FRONT);

    block_mine(block);

    printf("Block mined with hash: ");
    print_hash(block->hash);
    printf("\n");

    if (block_is_valid(block, prev, bc->unspent) == 0)
    {
        printf("Block is valid\n");
        llist_add_node(bc->chain, block, ADD_NODE_REAR);
        return block;
    }
    else
    {
        printf("Block is INVALID!\n");
        block_destroy(block);
        return NULL;
    }
}

int main(void)
{
    EC_KEY *miner = ec_create();
    if (!miner)
    {
        fprintf(stderr, "Failed to create miner key\n");
        return EXIT_FAILURE;
    }

    blockchain_t *bc = blockchain_create();
    if (!bc)
    {
        fprintf(stderr, "Failed to create blockchain\n");
        EC_KEY_free(miner);
        return EXIT_FAILURE;
    }

    // Genesis block is automatically created inside blockchain_create,
    // but if not, create one here manually and add it to bc->chain.

    block_t *prev = llist_get_head(bc->chain);
    if (!prev)
    {
        fprintf(stderr, "Blockchain has no genesis block\n");
        blockchain_destroy(bc);
        EC_KEY_free(miner);
        return EXIT_FAILURE;
    }

    printf("Genesis block at index %u with hash: ", prev->info.index);
    print_hash(prev->hash);
    printf("\n");

    // Add a couple of blocks
    block_t *block1 = add_block(bc, prev, "Block 1 data", miner);
    if (!block1)
    {
        fprintf(stderr, "Failed to add block 1\n");
        blockchain_destroy(bc);
        EC_KEY_free(miner);
        return EXIT_FAILURE;
    }

    block_t *block2 = add_block(bc, block1, "Block 2 data", miner);
    if (!block2)
    {
        fprintf(stderr, "Failed to add block 2\n");
        blockchain_destroy(bc);
        EC_KEY_free(miner);
        return EXIT_FAILURE;
    }

    // Clean up
    blockchain_destroy(bc);
    EC_KEY_free(miner);

    printf("Test complete\n");
    return EXIT_SUCCESS;
}

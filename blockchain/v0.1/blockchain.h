#ifndef BLOCKCHAIN_MAIN
#define BLOCKCHAIN_MAIN

#include <openssl/sha.h>
#include "provided/endianness.h"
#include <llist.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define BLOCKCHAIN_DATA_MAX 1024
#define GEN_HASH 00000000000000000000000000000000
#define GEN_TIMESTAMP 1537578000
#define HOLBERTON_HASH \
	"\xc5\x2c\x26\xc8\xb5\x46\x16\x39\x63\x5d\x8e\xdf\x2a\x97\xd4\x8d" \
	"\x0c\x8e\x00\x09\xc8\x17\xf2\xb1\xd3\xd7\xff\x2f\x04\x51\x58\x03"
#define MAGIC "\x48\x42\x4c\x4b"
#define VERSION "0.1"
#define HBLK_ENDIAN ((_get_endianness() == 1) ? "1" : "2")
#define DATA "Holberton School"
#define BLOCK_OFFSET 0xC

/**
 * struct blockchain_s - Blockchain structure
 *
 * @chain: Linked list of pointers to block_t
 */
typedef struct blockchain_s
{
	llist_t     *chain;
} blockchain_t;


/**
 * struct block_info_s - Block info structure
 *
 * @index:      Index of the Block in the Blockchain
 * @difficulty: Difficulty of proof of work (hash leading zero bits)
 * @timestamp:  Time the Block was created at (UNIX timestamp)
 * @nonce:      Salt value used to alter the Block hash
 * @prev_hash:  Hash of the previous Block in the Blockchain
 */
typedef struct block_info_s
{
    /*
     * The order of the elements in this structure should remain the same.
     * It was designed so every element of this structure is aligned and
     * doesn't require padding from the compiler.
     * Therefore, it is possible to use the structure as an array of char,
     * on any architecture.
     */
	uint32_t    index;
	uint32_t    difficulty;
	uint64_t    timestamp;
	uint64_t    nonce;
	uint8_t     prev_hash[SHA256_DIGEST_LENGTH];
} block_info_t;


/**
 * struct block_data_s - Block data
 *
 * @buffer: Data buffer
 * @len:    Data size (in bytes)
 */
typedef struct block_data_s
{
    /*
     * @buffer must stay first, so we can directly use the structure as
     * an array of char
     */
	uint8_t     buffer[BLOCKCHAIN_DATA_MAX];
	uint32_t    len;
} block_data_t;


/**
 * struct block_s - Block structure
 *
 * @info: Block info
 * @data: Block data
 * @hash: 256-bit digest of the Block, to ensure authenticity
 */
typedef struct block_s
{
	block_info_t    info; /* This must stay first */
	block_data_t    data; /* This must stay second */
	uint8_t     hash[SHA256_DIGEST_LENGTH];
} block_t;

/**
 * struct blockchain_header_s - blockchain header structure
 *
 * @hblk_magic:   magic number
 * @hblk_version: version number
 * @hblk_endian:  endianness (1 or 2)
 * @hblk_blocks:  number of blocks in chain
 */

typedef struct __attribute__((__packed__)) blockchain_header_s
{
	uint32_t hblk_magic;    /* HBLK or 48 42 4c 4b*/
	uint8_t  hblk_version[3];  /* X.Y, both are 0-9 ex. 1.5 */
	uint8_t  hblk_endian;   /* 1 little (reverse order) 2 big (in order) endian*/
	uint32_t hblk_blocks;   /* num blocks, blocks begin at 0xC*/
} blockchain_header_t;

/**
 * struct block_file_s - serialized version of blocks
 *
 * @index:       block index in the blockchain
 * @difficulty:  difficutly
 * @timestamp:   timestamp
 * @nonce:       nonce
 * @prev_hash:   previous hash
 * @data_len:    length of the data
 * @hash:        the hash itself
 */

typedef struct __attribute__((__packed__)) block_file_s
{
	uint32_t index;
	uint32_t difficulty;
	uint64_t timestamp;
	uint64_t nonce;
	uint8_t  prev_hash[SHA256_DIGEST_LENGTH];
	uint32_t data_len;
	uint8_t  data[BLOCKCHAIN_DATA_MAX];
} block_file_t;



blockchain_t *blockchain_create(void);
block_t *block_create(block_t const *prev,
		      int8_t const *data,
		      uint32_t data_len);
void block_destroy(block_t *block);
void blockchain_destroy(blockchain_t *blockchain);
uint8_t *block_hash(block_t const *block,
		    uint8_t hash_buf[SHA256_DIGEST_LENGTH]);
int blockchain_serialize(blockchain_t const *blockchain, char const *path);
blockchain_t *blockchain_deserialize(char const *path);
int block_is_valid(block_t const *block, block_t const *prev_block);


#endif

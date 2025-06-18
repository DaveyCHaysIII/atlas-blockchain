#include "blockchain.h"

blockchain_t *deserializer(FILE * fd,
			   uint8_t endian,
			   uint32_t numblocks);
void block_t_bitswapper(block_info_t *block);

/**
 * blockchain_deserialize - loads a blockchain from a file
 * @path: the location of the file
 *
 * Return: pointer to the blockchain_t or NULL
 */

blockchain_t *blockchain_deserialize(char const *path)
{

	FILE *fd;
	blockchain_header_t header;
	uint8_t endian;
	uint32_t numblocks;

	if (!path || access(path, F_OK) != 0 || access(path, R_OK) != 0)
		return (NULL);

	fd = fopen(path, "r");
	if (fd == NULL)
		return (NULL);

	if (fread(&header, sizeof(header), 1, fd) != 1)
		return (NULL);

	if (memcmp(header.hblk_magic, MAGIC, 4) != 0 ||
	    memcmp(header.hblk_version, VERSION, 3) != 0)
		return (NULL);

	endian = header.hblk_endian;
	numblocks = header.hblk_blocks;
	if (endian == 2)
		SWAPENDIAN(numblocks);

	return (deserializer(fd, endian, numblocks));
}

/**
 * deserializer - deserializes a valid hblk file
 * @fd: an accessable file descriptor of path
 * @endian: endiannes flag, 1 or 2
 * @numblocks: number of blocks in chain
 *
 * Return: pointer to new blockchain_t
 */

blockchain_t *deserializer(FILE *fd,
			   uint8_t endian,
			   uint32_t numblocks)
{
	blockchain_t *blockchain;
	block_t *block;

	blockchain = malloc(sizeof(*blockchain));
	blockchain->chain = llist_create(MT_SUPPORT_FALSE);

	while (numblocks != 0)
	{
		block = calloc(1, sizeof(*block));
		fread(block, sizeof(block_info_t), 1, fd);
		if (endian == 2)
			block_t_bitswapper(&block->info);
		fread(&block->data.len, sizeof(uint32_t), 1, fd);
		if (endian == 2)
			SWAPENDIAN(block->data.len);
		fread(block->data.buffer, block->data.len, 1, fd);
		fread(block->hash, sizeof(block->hash), 1, fd);
		llist_add_node((llist_t *)blockchain->chain,
			       block,
			       ADD_NODE_REAR);
		numblocks--;
	}
	fclose(fd);
	return (blockchain);
}

/**
 * block_t_bitswapper - swaps all the multi-byte members of a block_t
 * @block: the block in question
 *
 * Return: no return
 */


void block_t_bitswapper(block_info_t *block)
{
	SWAPENDIAN(block->index);
	SWAPENDIAN(block->difficulty);
	SWAPENDIAN(block->timestamp);
	SWAPENDIAN(block->nonce);
}

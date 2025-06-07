#include "blockchain.h"

int block_file_write(llist_node_t block, unsigned int index, void *fd);

/**
 * blockchain_serialize - serializes (saves) the struct to a file
 * @blockchain: pointer to the structure to save
 * @path: path to save the file to
 *
 * Return: 0 on success, -1 on failure
 */

int blockchain_serialize(blockchain_t const *blockchain, char const *path)
{
	FILE *fd;
	blockchain_header_t header;
	int blk_count;

	if (!blockchain || !path)
		return (-1);

	fd = fopen(path, "w");
	if (!fd)
		return (-1);

	memcpy((void *)&header.hblk_magic, MAGIC, 4);
	memcpy((void *)&header.hblk_version, VERSION, 3);
	header.hblk_endian = _get_endianness();
	blk_count = llist_size(blockchain->chain);
	memcpy(&header.hblk_blocks, &blk_count, sizeof(uint32_t));

	if (fwrite(&header, sizeof(header), 1, fd) != 1)
	{
		fprintf(stderr, "Header won't write\n");
		fclose(fd);
		return (-1);
	}

	if (llist_for_each(blockchain->chain, block_file_write, fd) != 0)
	{
		fprintf(stderr, "Blocks won't write\n");
		fclose(fd);
		return (-1);
	}

	fclose(fd);
	return (0);
}

/**
 * block_file_write - writes a block to a file
 * @block: the block in question
 * @index: the index of the block
 * @fd: the file descriptor to write it to
 *
 * Return: 0 on success, -1 on failure
 */

int block_file_write(llist_node_t block,
		     __attribute__((unused)) uint32_t index,
		     void *fd)
{
	uint8_t block_buff[1116] = {0};
	uint32_t block_len, block_data_len;

	if (!block || !fd)
		return (-1);
	if (_get_endianness() == 2)
		SWAPENDIAN(block);

	block_len = 0;      /* tracks size of block for final write */
	block_data_len = 0; /* holds size of block.data.len */

	/* copies all of the block_info_t */
	memcpy(block_buff, block, sizeof(block_info_t));
	block_data_len = ((block_t *)block)->data.len;

	/* just copies the block.data.len */
	memcpy(&block_buff[block_len += sizeof(block_info_t)],
	       &block_data_len,
	       sizeof(uint32_t));

	/* copies the actual data */
	memcpy(&block_buff[block_len += sizeof(uint32_t)],
	       ((block_t *)block)->data.buffer,
	       block_data_len);

	/* copies the hash */
	memcpy(&block_buff[block_len += block_data_len],
	       ((block_t *)block)->hash,
	       SHA256_DIGEST_LENGTH);

	fwrite(block_buff, block_len += SHA256_DIGEST_LENGTH, 1, (FILE *)fd);
	return (0);
}

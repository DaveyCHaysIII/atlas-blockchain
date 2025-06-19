#include "blockchain.h"

/**
 * hash_matches_difficulty - checks the difficulty leading bits of hash
 * @hash: the hash in question
 * @difficulty: number of leading bits to check
 *
 * Return: 1 on success, 0 on failure
 */

int hash_matches_difficulty(uint8_t const hash[SHA256_DIGEST_LENGTH],
			    uint32_t difficulty)
{
	uint32_t bytes, remain, i;
	uint8_t mask;

	if (!hash || !difficulty)
		return (0);

	bytes = difficulty / 8;
	remain = difficulty % 8;
	mask = 0xFF << (8 - remain);

	for (i = 0; i < bytes; i++)
	{
		if (hash[i] != 0)
			return (0);
	}

	if (remain != 0)
	{
		if ((hash[i] & mask) != 0)
			return (0);
	}

	return (1);
}

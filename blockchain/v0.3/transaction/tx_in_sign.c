#include "transaction.h"


/**
 * match_unspent - helper function to find unspent
 * @node: pointer to member of unspent list
 * @hash: pointer to the tx_in_t out_hash
 *
 * Return: -1 on fail, 0 on success
 */
static int match_unspent(void *node, void *hash)
{
	unspent_tx_out_t *out = node;

	if (memcmp(out->out.hash,
		    hash,
		    SHA256_DIGEST_LENGTH))
		return (0);

	return (-1);
}



/**
 * tx_in_sign - signs a transaction tx_in_t
 * @in: pointer to the transaction in
 * @tx_id: id of the transaction in
 * @sender: the private key of the account holder
 * @all_unspent: pointer to the unspent list
 *
 * Return: pointer to signed sig_t or NULL
 */

sig_t *tx_in_sign(tx_in_t *in,
		  uint8_t const tx_id[SHA256_DIGEST_LENGTH],
		  EC_KEY const *sender,
		  llist_t *all_unspent)
{
	unspent_tx_out_t *utxo;
	uint8_t derived_pub[EC_PUB_LEN];
	unsigned int sig_len;

	if (!in || !tx_id || !sender || !all_unspent)
	{
		printf("1\n");
		return (NULL);
	}

	sig_len = 0;
	utxo = NULL;

	utxo = llist_find_node(all_unspent, match_unspent, in->tx_out_hash);
	if (!utxo)
	{
		printf("2\n");
		return (NULL);
	}

	if (!ec_to_pub(sender, derived_pub))
	{
		printf("3\n");
		return (NULL);
	}

	if (memcmp(derived_pub, utxo->out.pub, EC_PUB_LEN) != 0)
	{
		printf("4\n");
		return (NULL);
	}

	if (!ECDSA_sign(0, tx_id, SHA256_DIGEST_LENGTH,
			in->sig.sig, &sig_len, (EC_KEY *)sender))
	{
		printf("5\n");
		return (NULL);
	}
	in->sig.len = (uint8_t)sig_len;
	return (&in->sig);
}

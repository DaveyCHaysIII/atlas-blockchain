#include "transaction.h"

/**
 * build_transaction - builds the transaction
 * @node: the node in the unspent list
 * @i: unused iter
 * @txt_data: transaction data struct
 *
 * Return: 0 or 1 when done
 */
static int build_transaction(llist_node_t node, unsigned int i, void *txt_data)
{
	(void)i;
	unspent_tx_out_t *utxo = node;
	tx_data_t *data = txt_data;
	tx_in_t *in;

	if (data->needed <= data->amount_total || !utxo)
		return (1);

	if (memcmp(data->pub, utxo->out.pub, EC_PUB_LEN) == 0)
	{
		in = tx_in_create(utxo);
		llist_add_node(data->txt->inputs, in, ADD_NODE_REAR);
		data->amount_total += utxo->out.amount;
	}
	return (0);
}

/**
 * sign_ins - signs all the ins
 * @node: the node in the ins list
 * @i: unused iter
 * @txt_data: transaction data struct
 *
 * Return: 0 or 1 when done
 */
static int sign_ins(llist_node_t node, unsigned int i, void *txt_data)
{
	(void)i;

	tx_in_t *in = node;
	tx_data_t *data = txt_data;

	if (!in)
		return (1);

	tx_in_sign(in,
		   data->txt->id,
		   data->sender,
		   data->all_unspent);
	return (0);
}

/**
 * transaction_create - creates a transaction_t
 * @sender: wallet of the sender
 * @receiver: wallet of the reciever
 * @amount: amount to transfer from sender to reciever
 * @all_unspent: pointer to list of all unspent
 *
 * Return: pointer to new transaction_t or NULL
 */

transaction_t *transaction_create(EC_KEY const *sender,
				  EC_KEY const *receiver,
				  uint32_t amount,
				  llist_t *all_unspent)
{
	transaction_t *transaction;
	tx_data_t *data;
	tx_out_t *out;
	uint8_t receiver_pub[EC_PUB_LEN];

	if (!sender || !receiver || !amount || !all_unspent)
		return (NULL);

	transaction = calloc(1, sizeof(transaction_t));
	data = calloc(1, sizeof(tx_data_t));
	ec_to_pub(sender, data->pub);
	ec_to_pub(receiver, receiver_pub);
	data->needed = amount;
	data->txt = transaction;
	data->sender = sender;
	data->all_unspent = all_unspent;
	transaction->inputs = llist_create(MT_SUPPORT_FALSE);
	transaction->outputs = llist_create(MT_SUPPORT_FALSE);
	llist_for_each(all_unspent, build_transaction, data);
	if (data->amount_total < data->needed)
	{
		llist_destroy(transaction->inputs, 1, free);
		llist_destroy(transaction->outputs, 1, free);
		free(transaction);
		free(data);
		return (NULL);
	}
	out = tx_out_create(data->needed, receiver_pub);
	llist_add_node(transaction->outputs, out, ADD_NODE_REAR);
	if (data->amount_total != data->needed)
	{
		out = tx_out_create(data->amount_total - data->needed,
				    data->pub);
		llist_add_node(transaction->outputs, out, ADD_NODE_REAR);
	}
	transaction_hash(transaction, transaction->id);
	llist_for_each(transaction->inputs, sign_ins, data);
	free(data);
	return (transaction);
}

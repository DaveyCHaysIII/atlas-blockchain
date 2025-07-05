#include "transaction.h"

/**
 * transaction_destroy - destroys a transaction
 * @transaction: the transaction to destroy
 *
 * Return: no return
 */

void transaction_destroy(transaction_t *transaction)
{
	int in_size, out_size;

	if (!transaction)
		return;

	in_size = llist_size(transaction->inputs);
	out_size = llist_size(transaction->outputs);

	if (in_size > 0)
		llist_destroy(transaction->inputs, 1, NULL);
	else
		llist_destroy(transaction->inputs, 0, NULL);

	if (out_size > 0)
		llist_destroy(transaction->outputs, 1, NULL);
	else
		llist_destroy(transaction->outputs, 0, NULL);

	free(transaction);
}

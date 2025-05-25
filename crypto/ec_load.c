#include "hblk_crypto.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/**
 * ec_load - loads a key from a file
 * @folder: the name of the folder its saved in
 *
 * Return: pointer to new EC_KEY or NULL
 */

EC_KEY *ec_load(char const *folder)
{
	struct stat st;
	EC_KEY *key;
	FILE *fd;
	char file_pub_path[1024];
	char file_priv_path[1024];
	char const *file_pub_name = "/key_pub.pem";
	char const *file_priv_name = "/key.pem";

	if (!folder)
		return (NULL);

	key == NULL;
	strcpy(file_priv_path, folder);
	strcpy(file_pub_path, folder);
	strcat(file_priv_path, file_priv_name);
	strcat(file_pub_path, file_pub_name);

	fd = fopen(file_pub_path, "w+");
	if (!fd)
		return (NULL);
	PEM_read_EC_PUBKEY(fd, &key, NULL, NULL);
	fclose(fd);

	fd = fopen(file_priv_path, "w+");
	if (!fd)
		return (NULL);
	PEM_read_ECPrivateKey(fd, key, NULL, NULL);
	fclose(fd);

	return (key);
}


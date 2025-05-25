#include "hblk_crypto.h"
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

/**
 * ec_save - saves a key to a file
 * @key: the key in question
 * @folder: the name of the folder to save it in
 *
 * Return: 0 or 1
 */

int ec_save(EC_KEY *key, char const *folder)
{
	struct stat st;
	FILE *fd;
	char file_pub_path[1024];
	char file_priv_path[1024];
	char const *file_pub_name = "/key_pub.pem";
	char const *file_priv_name = "/key.pem";

	if (!key || !folder)
		return (-1);

	if (stat(folder, &st) != 0 || !S_ISDIR(st.st_mode))
		mkdir(folder, 0755);

	strcpy(file_priv_path, folder);
	strcpy(file_pub_path, folder);
	strcat(file_priv_path, file_priv_name);
	strcat(file_pub_path, file_pub_name);

	fd = fopen(file_pub_path, "w+");
	PEM_write_EC_PUBKEY(fd, key);
	fclose(fd);

	fd = fopen(file_priv_path, "w+");
	PEM_write_ECPrivateKey(fd, key, NULL, NULL, 0, NULL, NULL);
	fclose(fd);

	return (0);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <net/if.h>
#include "netdev.h"
#include "mscp.h"
#include "print.h"
#include "strerrno.h"

static struct netdev_list *netdev_list = NULL;

struct netdev_list *get_netdev_list(struct mscp *m)
{
	struct netdev_list *list = NULL;
	struct netdev *dev;
	char *devices, *token, *saveptr;
	int index;

	if (!m || !m->opts || !m->opts->netdev) {
		pr_err("invalid arguments");
		return NULL;
	}

	list = calloc(1, sizeof(struct netdev_list));
	if (!list) {
		pr_err("calloc: %s", strerrno());
		return NULL;
	}

	devices = strdup(m->opts->netdev);
	if (!devices) {
		pr_err("strdup: %s", strerrno());
		free(list);
		return NULL;
	}

	for (token = strtok_r(devices, ",", &saveptr); token; token = strtok_r(NULL, ",", &saveptr)) {
		dev = calloc(1, sizeof(struct netdev));
		if (!dev) {
			pr_err("calloc: %s", strerrno());
			goto err;
		}

		dev->name = strdup(token);
		if (!dev->name) {
			pr_err("strdup: %s", strerrno());
			free(dev);
			goto err;
		}

		index = if_nametoindex(token);
		if (index == 0) {
			pr_err("if_nametoindex: %s", strerrno());
			free(dev->name);
			free(dev);
			goto err;
		}
		dev->index = index;

		list->devs = realloc(list->devs, sizeof(struct netdev *) * (list->nr_devs + 1));
		if (!list->devs) {
			pr_err("realloc: %s", strerrno());
			free(dev->name);
			free(dev);
			goto err;
		}

		list->devs[list->nr_devs++] = dev;
	}

	free(devices);
	return list;

err:
	free(devices);
	if (list) {
		for (int i = 0; i < list->nr_devs; i++) {
			if (list->devs[i]) {
				free(list->devs[i]->name);
				free(list->devs[i]);
			}
		}
		free(list->devs);
		free(list);
	}
	return NULL;
}

void free_netdev_list(struct netdev_list *list)
{
	if (!list)
		return;

	for (int i = 0; i < list->nr_devs; i++) {
		if (list->devs[i]) {
			free(list->devs[i]->name);
			free(list->devs[i]);
		}
	}
	free(list->devs);
	free(list);
}

int bind_socket_to_netdev(int sock, const char *devname)
{
	int index;

	if (!devname) {
		pr_err("invalid arguments");
		return -1;
	}

	index = if_nametoindex(devname);
	if (index == 0) {
		pr_err("if_nametoindex: %s", strerrno());
		return -1;
	}

	if (setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, devname, strlen(devname)) < 0) {
		pr_err("setsockopt: %s", strerrno());
		return -1;
	}

	return 0;
}

const char *get_netdev_by_index(int index)
{
	struct netdev_list *list;
	int i;

	if (!netdev_list) {
		list = get_netdev_list(NULL);
		if (!list)
			return NULL;
		netdev_list = list;
	}

	for (i = 0; i < netdev_list->nr_devs; i++) {
		if (netdev_list->devs[i]->index == index)
			return netdev_list->devs[i]->name;
	}

	return NULL;
}

int get_netdev_count(void)
{
	struct netdev_list *list;

	if (!netdev_list) {
		list = get_netdev_list(NULL);
		if (!list)
			return 0;
		netdev_list = list;
	}

	return netdev_list->nr_devs;
}
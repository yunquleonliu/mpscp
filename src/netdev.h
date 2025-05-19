#ifndef NETDEV_H
#define NETDEV_H

#include "mscp.h"

struct netdev {
    char *name;
    int index;
};

struct netdev_list {
    struct netdev **devs;
    int nr_devs;
};

struct netdev_list *get_netdev_list(struct mscp *m);
void free_netdev_list(struct netdev_list *list);
int bind_socket_to_netdev(int sock, const char *devname);
const char *get_netdev_by_index(int index);
int get_netdev_count(void);

#endif /* NETDEV_H */ 
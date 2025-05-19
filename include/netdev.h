#ifndef NETDEV_H
#define NETDEV_H

#include <stddef.h>

struct netdev {
    char name[16];
    int index;
    struct netdev *next;
};

extern struct netdev *netdev_list;

int get_netdev_list(void);
int get_netdev_count(void);
const char *get_netdev_by_index(int index);
int bind_socket_to_netdev(int sock, const char *devname);
void free_netdev_list(void);

#endif /* NETDEV_H */ 
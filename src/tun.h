// Create, manage, read from and write to a TUN interface
// TUN is a virtual network interface that tunnels L3 packets to userspace.

#ifndef TUN
#define TUN

#include "linux/if.h"

struct tun_info
{
    int fd;
    int err;
    struct ifreq ifr;
};

struct tun_info *create_tun();
void free_tun(struct tun_info *tun);

#endif
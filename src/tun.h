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
int set_ip_address(struct tun_info *tun, const char *address);
int set_subnet_mask(struct tun_info *tun, const char *mask);
void get_network_address(const char *addr, const char *mask, char *buf);
void free_tun(struct tun_info *tun);

#endif
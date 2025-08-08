/**
 * Create, manage, read from and write to a TUN interface
 * TUN is a virtual network interface that tunnels L3 packets to userspace.
 */

#include "fcntl.h"
#include "linux/if.h"
#include "linux/if_tun.h"
#include "netinet/in.h"
#include "stdlib.h"
#include "string.h"
#include "sys/ioctl.h"
#include "sys/socket.h"
#include "unistd.h"
#include <arpa/inet.h>

#include "logger.h"
#include "tun.h"

/**
 * @brief Create a new tun virtual interface.
 *
 * @return Pointer to struct containing info of created interface.
 */
struct tun_info *create_tun()
{
    struct tun_info *tun = (struct tun_info *)malloc(sizeof(struct tun_info));
    memset(&tun->ifr, 0, sizeof(struct ifreq));

    // tun special device has this name
    const char *spec_dev = "/dev/net/tun";

    // Open the special device
    tun->fd = open(spec_dev, O_RDWR);
    if (tun->fd < 0)
    {
        return tun;
    }
    log_debug("Opened tun file: %d", tun->fd);

    // Request to create tun interface
    tun->ifr.ifr_flags = IFF_TUN;
    tun->err = ioctl(tun->fd, TUNSETIFF, &tun->ifr);
    if (tun->err < 0)
    {
        close(tun->fd);
        return tun;
    }
    log_debug("Created tun interface: %s", tun->ifr.ifr_name);

    return tun;
}

/**
 * @brief Assign an IPv4 address to a tun interface.
 *
 * @param tun Pointer to struct of tun info.
 */
int set_ip_address(struct tun_info *tun, const char *address)
{
    // ioctl needs a dummy socket to assign an IP address to an interface.
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    log_debug("Setting IP for tun interface: %s", tun->ifr.ifr_name);

    // Create binary representation of IP address and assign to socket
    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = 0;
    inet_pton(AF_INET, address, &sock_addr.sin_addr);

    // Assign IP address representation to interface
    tun->ifr.ifr_addr = *(struct sockaddr *)&sock_addr;
    return ioctl(sockfd, SIOCSIFADDR, &tun->ifr);
}

/**
 * @brief Close resources related to a tun virtual interface
 *
 * @param tun Pointer to struct of tun info.
 */
void free_tun(struct tun_info *tun)
{
    close(tun->fd);
    free(tun);
}
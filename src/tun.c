/**
 * @file tun.c
 * @brief Create, manage, read from and write to a TUN interface
 *
 * TUN is a virtual network interface that tunnels L3 packets to userspace.
 * See for more: https://docs.kernel.org/networking/tuntap.html
 */

#include "fcntl.h"
#include "linux/if.h"
#include "linux/if_tun.h"
#include "net/route.h"
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
 * @param address IP address string.
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
    int result = ioctl(sockfd, SIOCSIFADDR, &tun->ifr);

    close(sockfd);
    return result;
}

/**
 * @brief Assign a subnet mask to a tun interface.
 *
 * @param tun Pointer to struct of tun info.
 * @param mask Subnet mask string.
 */
int set_subnet_mask(struct tun_info *tun, const char *mask)
{
    // Basically the same implementation as set_ip address above
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    log_debug("Setting subnet mask for tun interface: %s", tun->ifr.ifr_name);

    struct sockaddr_in sock_addr;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));
    sock_addr.sin_family = AF_INET;
    sock_addr.sin_port = 0;
    inet_pton(AF_INET, mask, &sock_addr.sin_addr);

    tun->ifr.ifr_netmask = *(struct sockaddr *)&sock_addr;
    int result = ioctl(sockfd, SIOCSIFNETMASK, &tun->ifr);

    close(sockfd);
    return result;
}

/**
 * @brief This is a helper function for the client to get
 * the network address of a subnet for it's routing table based on the
 * address and subnet mask the interface lives in.
 *
 * It basically just performs a bitwise AND between the
 * address and the mask.
 *
 * @param addr String form of address.
 * @param mask String form of net mask.
 * @param buf Buffer to hold result - MUST have at least size INET_ADDRSTRLEN
 *
 * @return String form of the subnet's network address.
 */
void get_network_address(const char *addr, const char *mask, char *buf)
{
    // Convert string to binary
    struct sockaddr_in sock_addr;
    struct sockaddr_in sock_mask;
    memset(&sock_addr, 0, sizeof(struct sockaddr_in));
    memset(&sock_mask, 0, sizeof(struct sockaddr_in));

    inet_pton(AF_INET, addr, &sock_addr.sin_addr);
    inet_pton(AF_INET, mask, &sock_mask.sin_addr);

    // Bitwise AND
    struct sockaddr_in sock_res;
    sock_res.sin_addr.s_addr = sock_addr.sin_addr.s_addr & sock_mask.sin_addr.s_addr;

    // Binary back to string;
    inet_ntop(AF_INET, &sock_res.sin_addr, buf, INET_ADDRSTRLEN);
}

/**
 * @brief Add routing table entry to direct traffic
 * to the tun interface appropriately.
 *
 * @param target_addr String form of target address for route.
 * @param target_mask String form of target net mask for route.
 */
void set_route(const char *target_addr, const char *target_mask)
{
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
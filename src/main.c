#include "errno.h"
#include "netinet/in.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "logger.h"
#include "tun.h"

const char TEST_ADDR[] = "10.0.0.1";
const char TEST_MASK[] = "255.255.255.0";

int main()
{

    // Attempt to create virtual tunnel for VPN forward/receive
    struct tun_info *vpn_tun = create_tun();
    if (vpn_tun->fd < 0 || vpn_tun->err < 0)
    {
        log_error("Could not create VPN tunnel interface - %s", strerror(errno));
        free_tun(vpn_tun);
        return -1;
    }

    // Configure tunnel
    if (set_ip_address(vpn_tun, TEST_ADDR) < 0)
    {
        log_error("Could not assign IP address - %s", strerror(errno));
        free_tun(vpn_tun);
        return -1;
    }

    if (set_subnet_mask(vpn_tun, TEST_MASK) < 0)
    {
        log_error("Could not assign Subnet Mask - %s", strerror(errno));
        free_tun(vpn_tun);
        return -1;
    }

    char net_address[INET_ADDRSTRLEN];
    get_network_address(TEST_ADDR, TEST_MASK, net_address);
    log_debug("Got network address - %s", net_address);

    // Main loop
    while (1)
    {
    };

    return 0;
}
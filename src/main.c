#include "errno.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "logger.h"
#include "tun.h"

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
    if (set_ip_address(vpn_tun, "10.0.0.1") < 0)
    {
        log_error("Could not assign IP address - %s", strerror(errno));
        free_tun(vpn_tun);
        return -1;
    }

    // Main loop
    while (1)
    {
    };

    return 0;
}
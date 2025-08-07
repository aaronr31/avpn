#include "errno.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "tun.h"

int main()
{

    // Attempt to create virtual tunnel for VPN forward/receive
    struct tun_info *vpn_tun = create_tun();
    if (vpn_tun->fd < 0 || vpn_tun->err < 0)
    {
        printf("FATAL: Could not create VPN tunnel interface\n%s\n", strerror(errno));
        free_tun(vpn_tun);
        return -1;
    }

    // Main loop
    while (1)
    {
    };

    return 0;
}
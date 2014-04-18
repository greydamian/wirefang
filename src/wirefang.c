/*
 * Copyright (c) 2014 Damian Jason Lapidge
 *
 * The contents of this file are subject to the terms and conditions defined 
 * within the file LICENSE.txt, located within this project's root directory.
 */

/*
 * stdio.h    : stderr, printf(), fprintf()
 * unistd.h   : geteuid(), close()
 * fcntl.h    : O_RDONLY, open()
 * sys/ioctl.h: SIOCGIFINDEX, ioctl()
 * string.h   : memset(), strncpy()
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>

/*
 * sys/socket.h     : AF_PACKET, SOCK_RAW, socket(), bind()
 * net/ethernet.h   : ETH_P_ALL
 * net/if.h         : IFNAMSIZ, struct ifreq
 * linux_if_packet.h: struct sockaddr_ll
 */
#include <sys/socket.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <linux/if_packet.h>

/*
 * greyio.h: readall(), writeall()
 */
#include "greyio.h"

static const char *version = "v1.0.0";
static const char *authors = "Damian Jason Lapidge <grey@greydamian.org>";

void print_usage() {
    fprintf(stderr, "wirefang <file> <interface>\n");
}

int create_rawsock(char *iface) {
    int sd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (sd == -1)
        return -1; /* failure */

    struct ifreq ifrqst;
    struct sockaddr_ll bindaddr;

    memset(&ifrqst, 0, sizeof(ifrqst));
    memset(&bindaddr, 0, sizeof(bindaddr));

    strncpy((char *)ifrqst.ifr_name, iface, IFNAMSIZ);
    if (ioctl(sd, SIOCGIFINDEX, &ifrqst) == -1) {
        close(sd);
        return -1; /* failure */
    }

    bindaddr.sll_family   = AF_PACKET;
    bindaddr.sll_ifindex  = ifrqst.ifr_ifindex;
    bindaddr.sll_protocol = htons(ETH_P_ALL);

    if (bind(sd, (struct sockaddr *)&bindaddr, sizeof(bindaddr)) == -1) {
        close(sd);
        return -1; /* failure */
    }

    return sd;
}

int main(int argc, char *argv[]) {
    if (geteuid() > 0) {
        fprintf(stderr, "error: this program requires superuser privilages\n");
        return 1; /* exit failure */
    }

    if (argc < 3) {
        print_usage();
        return 1; /* exit failure */
    }

    char *fpath = argv[argc - 2];
    char *iface = argv[argc - 1];

    int sd = create_rawsock(iface);
    if (sd == -1) {
        fprintf(stderr, "error: failure to create network connection\n");
        return 1; /* exit failure */
    }

    int fd = open(fpath, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "error: failure to open file (%s)\n", fpath);
        return 1; /* exit failure */
    }

    char *buf = NULL;
    ssize_t rbytes = readall(fd, (void **)&buf);
    if (rbytes == -1) {
        fprintf(stderr, "error: failure reading from file\n");
        return 1; /* exit failure */
    }
    close(fd); /* no further use for file */

    ssize_t wbytes = writeall(sd, buf, rbytes);
    if (wbytes == -1) {
        fprintf(stderr, "error: failure writing packet to network\n");
        return 1; /* exit failure */
    }
    close(sd);

    return 0; /* exit success */
}


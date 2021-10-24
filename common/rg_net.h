/*
 * rg_net.h
 *
 *  Created on: 7 апр. 2021 г.
 *      Author: alex9932
 */

#ifndef RG_NET_H_
#define RG_NET_H_

#include <SDL2/SDL_net.h>

#define RG_NET_PKG_TYPE_NULL       0x00
#define RG_NET_PKG_TYPE_PING       0x01
#define RG_NET_PKG_TYPE_DATA       0x02
#define RG_NET_PKG_TYPE_DISCONNECT 0x03
#define RG_NET_PKG_TYPE_FAILED     0xFF

#define RG_NET_MAX_LENGTH 1024

typedef struct {
	unsigned char pkg_type;
	Uint32 length;
	void* data;
} rg_NetPackage;

void rg_netInit();
void rg_netQuit();

TCPsocket rg_netOpenServer(Uint16 port);
TCPsocket rg_netConnect(const char* addr, Uint16 port);
TCPsocket rg_netAccept(TCPsocket socket);
void rg_netClose(TCPsocket socket);

void rg_netSend(TCPsocket socket, rg_NetPackage* pkg);
rg_NetPackage* rg_netRecv(TCPsocket socket);
void rg_netFreePackage(rg_NetPackage* pkg);

#endif /* RG_NET_H_ */

/*
 * rg_net.cpp
 *
 *  Created on: 7 апр. 2021 г.
 *      Author: alex9932
 */

#include "rg_net.h"
#include "rg_engine.h"
#include <SDL2/SDL.h>

struct _rg_pkgHeader {
	unsigned char pkg_type;
	Uint32 length;
};

void rg_netInit() {
	SDL_LogInfo(SDL_LOG_CATEGORY_NETWORK, "Starting networking subsystem...");
	if (SDLNet_Init() == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "Filed!");

	}
	SDL_LogInfo(SDL_LOG_CATEGORY_NETWORK, "SDLNet ver: %d.%d.%d", SDLNet_Linked_Version()->major, SDLNet_Linked_Version()->minor, SDLNet_Linked_Version()->patch);

}

void rg_netQuit() {
	SDLNet_Quit();
}

TCPsocket rg_netOpenServer(Uint16 port) {
	static IPaddress ip;
	if(SDLNet_ResolveHost(&ip, NULL, port) == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "Error: %s", SDLNet_GetError());
	    return NULL;
	}

	TCPsocket tcpsock = SDLNet_TCP_Open(&ip);
	if(!tcpsock) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "Error: %s", SDLNet_GetError());
	    return NULL;
	}

	return tcpsock;
}

TCPsocket rg_netConnect(const char* addr, Uint16 port) {
	IPaddress ip;
	TCPsocket tcpsock;

	if(SDLNet_ResolveHost(&ip, addr, port) == -1) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "Error: %s", SDLNet_GetError());
	    return NULL;
	}

	tcpsock = SDLNet_TCP_Open(&ip);
	if(!tcpsock) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "Error: %s", SDLNet_GetError());
	    return NULL;
	}

	return tcpsock;
}

TCPsocket rg_netAccept(TCPsocket socket) {
	return SDLNet_TCP_Accept(socket);
}

void rg_netClose(TCPsocket socket) {
	SDLNet_TCP_Close(socket);
}

void rg_netSend(TCPsocket socket, rg_NetPackage* pkg) {
	_rg_pkgHeader header;
	header.pkg_type = pkg->pkg_type;
	header.length = pkg->length;

//	if((Uint32)SDLNet_TCP_Send(socket, pkg, sizeof(rg_NetPackage)) < sizeof(rg_NetPackage)) {
	if((Uint32)SDLNet_TCP_Send(socket, &header, sizeof(_rg_pkgHeader)) < sizeof(_rg_pkgHeader)) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "hSent failed!");
		pkg->pkg_type = RG_NET_PKG_TYPE_FAILED;
	}
	if((Uint32)SDLNet_TCP_Send(socket, pkg->data, pkg->length) < pkg->length) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "dSent failed!");
		pkg->pkg_type = RG_NET_PKG_TYPE_FAILED;
	}
}

rg_NetPackage* rg_netRecv(TCPsocket socket) {
	rg_NetPackage* pkg = (rg_NetPackage*)rg_malloc(sizeof(rg_NetPackage));
	_rg_pkgHeader header;

	//	if (SDLNet_TCP_Recv(socket, pkg, sizeof(rg_NetPackage)) <= 0) {

//	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "P1");
	if (SDLNet_TCP_Recv(socket, &header, sizeof(_rg_pkgHeader)) <= 0) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "hRecv failed!");
		pkg->pkg_type = RG_NET_PKG_TYPE_FAILED;
	}

	pkg->pkg_type = header.pkg_type;
	pkg->length = header.length;

	pkg->data = rg_malloc(pkg->length);
//	SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "P2");
	if (SDLNet_TCP_Recv(socket, pkg->data, pkg->length) <= 0) {
		SDL_LogError(SDL_LOG_CATEGORY_NETWORK, "dRecv failed!");
		pkg->pkg_type = RG_NET_PKG_TYPE_FAILED;
	}
	return pkg;
}

void rg_netFreePackage(rg_NetPackage* pkg) {
	rg_free(pkg->data);
	rg_free(pkg);
}

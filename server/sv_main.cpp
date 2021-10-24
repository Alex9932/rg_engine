/*
 * sv_main.cpp
 *
 *  Created on: 30 мар. 2021 г.
 *      Author: alex9932
 */

#include "server.h"
#include <SDL2/SDL.h>
#include <rg_engine.h>
#include <rg_string.h>
#include <rg_vecmath.h>
#include <rg_net.h>

#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include <SDL2/SDL_net.h>

static TCPsocket sock;
static float r = 0;
static int cid = 0;

typedef struct SV_Client {
	Uint32 client_id;
	TCPsocket socket;
	vec4 player;
} SV_Client;

static std::vector<SV_Client*> clients;

static rg_string _serverclosed = "Disconnected by server: Server closed!";

static bool _sv_event_handler(rg_Event* event) {
	switch (event->type) {
		case RG_EVENT_SHUTDOWN: {
			SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Disconnecting clients...");
			for (Uint32 i = 0; i < clients.size(); ++i) {
				rg_NetPackage pkg;
				pkg.pkg_type = RG_NET_PKG_TYPE_DISCONNECT;
				pkg.length = SDL_strlen(_serverclosed);
				pkg.data = (void*)_serverclosed;
				rg_netSend(clients[i]->socket, &pkg);
			}
			break;
		}
		default:
			break;
	}

//	rg_print("sv: Event handled!");

	return true;
}

static int sv_cmd(void* data) {
	char cmd[128];
	while(rg_isRunning()) {
		char* str = fgets(cmd, sizeof(cmd), stdin);
		if(str == NULL) {
			SDL_Log("Error while reading command!");
		} else {
			rg_string_t* r_str = rg_strdup(cmd);
			SDL_Log("Handling: %s", cmd);

			if(rg_streql(r_str, L"quit\n")) {
				rg_stop();
			}

			rg_strfree(r_str);
		}

	}
	return 0;
}

static int sv_handler(void* data) {
	SV_Client* client = (SV_Client*)data;
	SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Client connected!");

	bool getPlayer = false;
	rg_NetPackage pkg;
	while(rg_isRunning()) {

		rg_NetPackage* r_pkg = rg_netRecv(client->socket); // Get request

		if(r_pkg->pkg_type == RG_NET_PKG_TYPE_FAILED) {
			rg_netFreePackage(r_pkg);
			SDL_LogInfo(SDL_LOG_CATEGORY_CLIENT, "Client closed!");
			break;
		} else if(r_pkg->pkg_type == RG_NET_PKG_TYPE_DISCONNECT) {
			rg_netFreePackage(r_pkg);
			SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Client closed!");
			break;
		} else if(r_pkg->pkg_type == RG_NET_PKG_TYPE_DATA) {
//			rg_string_t* str = rg_strdup((const char*)r_pkg->data);

			pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
			pkg.length = 4;
			pkg.data = (char*)"PING";

			if(getPlayer) {
				vec4* vec = (vec4*)r_pkg->data;
				client->player.x = vec->x;
				client->player.y = vec->y;
				client->player.z = vec->z;
				client->player.w = vec->w;
				SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Player%d at: %f %f %f", client->client_id, client->player.x, client->player.y, client->player.z);
				getPlayer = false;
			} else {
				const char* str = (const char*)r_pkg->data;

				if(rg_streql(str, "GET")) {
					pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
					pkg.length = sizeof(float);
					pkg.data = &r;
				}
				if(rg_streql(str, "PLAYER")) {
					pkg.pkg_type = RG_NET_PKG_TYPE_DATA;
					pkg.length = 2;
					pkg.data = (char*)"OK";
					getPlayer = true;
				}
			}

			rg_netSend(client->socket, &pkg);
			if(pkg.pkg_type == RG_NET_PKG_TYPE_FAILED) {
				SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "FAILED!");
				break;
			}

//			rg_strfree(str);
		}
		rg_netFreePackage(r_pkg);
		SDL_Delay(20);
	}

	SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Client disconnected!");
	clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
//	SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Memory: %d, free at: 0x%x", rg_getAllocatedMem(), client);
	rg_free(client);
	return 0;
}

static int _sv_netHandler(void* data) {
	while (rg_isRunning()) {
		TCPsocket socket = rg_netAccept(sock);
		if(socket) {
			SDL_LogInfo(SDL_LOG_CATEGORY_NETWORK, "Accepted client connection: %d", cid);
			SV_Client* client = (SV_Client*)rg_malloc(sizeof(SV_Client));
//			SDL_LogInfo(SDL_LOG_CATEGORY_SYSTEM, "Memory: %d, alloc at: 0x%x", rg_getAllocatedMem(), client);
			client->socket = socket;
			client->client_id = ++cid;
			clients.push_back(client);
			SDL_CreateThread(sv_handler, "Client handler", client);
		}
	}

	return 0;
}

void sv_quit() {

}

void sv_main() {
	SDL_LogInfo(SDL_LOG_CATEGORY_SERVER, "Initializing server");
	rg_registerEventHandler(_sv_event_handler);
	sock = rg_netOpenServer(12345);

	SDL_CreateThread(_sv_netHandler, "Net handler", NULL);
	SDL_CreateThread(sv_cmd, "Command listener", NULL);
}

void sv_update(double dt) {
	r += 0.4 * dt;
}

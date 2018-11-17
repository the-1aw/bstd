/*
** EPITECH PROJECT, 2018
** tech3
** File description:
** AUDPConnection
*/

#ifdef _WIN32
	#include "WinSocket.hpp"
#else
	#include "UnixSocket.hpp"
#endif

#include <iostream>
#include <string>

#pragma once

namespace bstd::network {
	class AUDPConnection : public Socket {
	public:
		AUDPConnection() : Socket(SOCK_DGRAM) {};

	public:
		std::string recvfrom(SOCKET sock, SOCKADDR_IN *sin = NULL,int flags = 0) const{
			uint32_t sinsize = sizeof(*sin);
			char buffer[READ_SIZE];
			std::string msg;
			int len;

			if ((len = ::recvfrom(sock, buffer, READ_SIZE - 1, flags, (SOCKADDR *)sin, &sinsize)) < 0) {
				std::cerr << "recvfrom failed" << std::endl;
			} else {
				buffer[len] = 0;
				msg = buffer;
			}
			return msg;
		};

		void sendto(SOCKET sock, std::string_view buffer, std::string_view host = DEFAULT_HOST, PORT port = 80, int flags = 0) const {
			struct hostent *hostinfo;
			SOCKADDR_IN sin = {0};

			if ((hostinfo = ::gethostbyname(host.data())) == NULL) {
				std::cerr << "Unknown host " << host << std::endl;
				return;
			}

			sin.sin_addr = *(IN_ADDR *) hostinfo->h_addr;
   			sin.sin_port = htons(port);
   			sin.sin_family = AF_INET;

			if(::sendto(sock, buffer.data(), buffer.size(), flags, (SOCKADDR *)&sin, sizeof(sin)) < 0)
				std::cerr << "Sendto() failed" << std::endl;
		};

		virtual void run() = 0;
		virtual ~AUDPConnection() {};

	private:
		static inline const int READ_SIZE = 4096;
	};

	class UDPBasicEchoServer : AUDPConnection {
	public:
		UDPBasicEchoServer(PORT port = 5000) : AUDPConnection(), _port(bind()) {
			std::cout << "UDP Server listening on port " << _port << ":" << std::endl;
		};

		void run() override {
			std::string msg;
			SOCKADDR_IN sin = {0};

			while (1) {
				msg = recvfrom(getSocket(), &sin);
				if (!msg.empty())
					std::cout << msg << std::endl;
			}
		};

		~UDPBasicEchoServer() {};
	public:
		PORT _port;
	};

	class UDPBasicEchoClient : AUDPConnection {
	public:
		UDPBasicEchoClient() : AUDPConnection() {};

		void run() override {
			std::string msg;

			while (1) {
				std::getline(std::cin, msg);
				sendto(getSocket(), msg, _dest, _destPort);
			}
		}

	public:
		void setDest(std::string_view str) {
			_dest = str;
		};

		void setDestPort(PORT port) {
			_destPort = port;
		}

	public:
		~UDPBasicEchoClient() {};

	private:
		std::string _dest;
		PORT _destPort;
	};
}
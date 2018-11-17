/*
** EPITECH PROJECT, 2018
** tech3
** File description:
** TCPServer
*/

#ifdef _WIN32
	#include "WinSocket.hpp"
#else
	#include "UnixSocket.hpp"
#endif

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <array>

#pragma once

namespace bstd::network {
	class ATCPConnection : public Socket {
	public:
		ATCPConnection() : Socket(SOCK_STREAM) {
		};

	public:
		void send(SOCKET sock, std::string_view buffer, size_t len, int flags = 0) const {
			if (::send(sock, buffer.data(), len, flags) < 0)
				std::cerr << "Send() failed" << std::endl;
		};

		const std::string revc(SOCKET sock, int flags = 0) const {
			char buffer[READ_SIZE] = {0};
			std::string str;
			int len;

			while ((len = ::recv(sock, buffer, READ_SIZE - 1, flags))) {
				if (len < 0)
					std::cerr << "Recv() failed" << std::endl;
				buffer[len] = 0;
				str += buffer;
				if (len < READ_SIZE - 1)
					break;
			}
			return str;
		};

		virtual void run() = 0;
		virtual ~ATCPConnection() {};

	private:
		static inline const int READ_SIZE = 4096;
	};

	class TCPBasicEchoServer : public ATCPConnection {
	public:
		TCPBasicEchoServer(size_t CONNECTION_MAX = 1, bool verbose = true) : ATCPConnection(), _port(bind()) {
			listen(CONNECTION_MAX);
			if(verbose)
				std::cout << "Listening on port " << _port << ":" << std::endl;
		};

		void run() override {
			std::string msg;

			while (1) {
				SOCKET csock;

				csock = this->accept(_port);
				while(1) {
					msg = this->revc(csock);
					std::cout << msg << std::endl;
					if (msg.size() == 0) {
						std::cout << "LE CLIENT EST MORT :(" << std::endl << "BYE BYE" << std::endl;
						break;
					}
				}
			}
		}

		~TCPBasicEchoServer() {};
	private:
		PORT _port;
	};

	class TCPBasicEchoClient : public ATCPConnection {
	public:
		TCPBasicEchoClient(PORT port, const std::string &host = DEFAULT_HOST) : ATCPConnection() {
			connect(port, host);
			std::cout << "Connected to " << host << ":" << port << std::endl;
		};

		void run() override {
			std::string msg;

			while (1) {
				std::getline(std::cin, msg);
				send(getSocket(), msg, msg.size());
			}
		}

		~TCPBasicEchoClient() {};
	};
}
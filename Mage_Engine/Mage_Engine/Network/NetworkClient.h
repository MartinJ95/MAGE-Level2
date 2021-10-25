#pragma once
#include <SFML/Network.hpp>
#include "Network/NetworkMessages.h"
#include <iostream>
#include <thread>
#include <mutex>

class World;

const unsigned short port = 53000;

class NetworkClient
{
public:
	sf::TcpSocket m_socket;
	int m_ID;
	char m_buffer[1024];
	size_t m_bufferSize;
	size_t m_bufferRecieved;
	World &m_worldRef;
	std::thread m_readerThread;
	std::mutex m_lock;
	bool m_connected;
	NetworkClient(World &world);
	void Initialize();
	void RecieveMessage();
	void SendMessage(Message *message);
	~NetworkClient();
};


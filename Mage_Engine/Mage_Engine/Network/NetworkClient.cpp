#include "NetworkClient.h"
#include "World.h"

NetworkClient::NetworkClient(World &world) :
	m_socket(),
	m_ID(-1),
	m_buffer(),
	m_bufferSize(sizeof(m_buffer)),
	m_worldRef(world),
	m_lock(),
	m_connected(false)
{
	
}

void NetworkClient::Initialize()
{
	sf::IpAddress ipAddress;
	std::cout << "enter ipAddress the server is running on" << std::endl;
	std::cin >> ipAddress;
	std::cout << "address : " << ipAddress << std::endl;
	if (m_socket.connect(ipAddress, port, sf::seconds(30)) == sf::Socket::Status::Done)
	{
		std::cout << "connected to server" << std::endl;
		//RecieveMessage();
		m_connected = true;
		m_readerThread = std::thread(&NetworkClient::RecieveMessage, this);
	}
	else
	{
		std::cout << "failed to connect" << std::endl;
	}
}

void NetworkClient::RecieveMessage()
{
	while (m_connected)
	{
		if (m_socket.receive(m_buffer, m_bufferSize, m_bufferRecieved) == sf::Socket::Done)
		{
			void *ptr = &m_buffer[0];
			Message *m = static_cast<Message*>(ptr);
			if (m->type == Connect)
			{
				std::cout << "connect message" << std::endl;
				std::cout << m->ID << std::endl;
				if (m_ID == -1)
				{
					//m_socket.setBlocking(false);
					m_ID = m->ID;
					std::cout << m_ID << std::endl;
				}
				else if (m->ID > -1)
				{
					for (int i = 0; i < m_worldRef.m_entities.size(); i++)
					{
						if (m_worldRef.m_entities[i]->m_active == false && m_worldRef.m_entities[i]->getComponent<RemoteClient>() != NULL)
						{
							m_worldRef.m_entities[i]->m_active = true;
							m_worldRef.m_entities[i]->getComponent<RemoteClient>()->m_ID = m->ID;
							std::cout << "remote player : " << m_worldRef.m_entities[i]->getComponent<RemoteClient>()->m_ID << " has joined" << std::endl;
							break;
						}
					}
				}
			}
			else if (m->type == MessageType::Disconnect)
			{
				for (int i = 0; i < m_worldRef.m_entities.size(); i++)
				{
					if (m_worldRef.m_entities[i]->m_active == true && m_worldRef.m_entities[i]->getComponent<RemoteClient>() != NULL)
					{
						if (m_worldRef.m_entities[i]->getComponent<RemoteClient>()->m_ID == m->ID)
						{
							m_worldRef.m_entities[i]->getComponent<RemoteClient>()->m_ID = -1;
							m_worldRef.m_entities[i]->m_active = false;
						}
					}
				}
			}
			else if (m->type == MessageType::PositionUpdate)
			{
				TransformUpdateMessage *tm = static_cast<TransformUpdateMessage*>(ptr);
				for (int i = 0; i < m_worldRef.m_entities.size(); i++)
				{
					if (m_worldRef.m_entities[i]->m_active == true && m_worldRef.m_entities[i]->getComponent<RemoteClient>() != NULL)
					{
						if (m_worldRef.m_entities[i]->getComponent<RemoteClient>()->m_ID == tm->ID)
						{
							m_worldRef.m_entities[i]->getComponent<Transform>()->m_position = tm->position;
							m_worldRef.m_entities[i]->getComponent<Transform>()->m_rotation = tm->rotation;
							m_worldRef.m_entities[i]->getComponent<Transform>()->m_scale = tm->scale;

							Transform *t = m_worldRef.m_entities[i]->getComponent<Transform>();
						}
					}
				}
			}
		}
	}
}

void NetworkClient::SendMessage(Message *message)
{
	m_lock.lock();
	if (message->type == MessageType::PositionUpdate)
	{
		void *ptr = message;
		//TransformUpdateMessage *m = (TransformUpdateMessage*)message;
		TransformUpdateMessage *m = static_cast<TransformUpdateMessage*>(ptr);
		std::cout << sizeof(TransformUpdateMessage) << std::endl;
		std::cout << sizeof(*m) << std::endl;
		m_socket.send(m, sizeof(TransformUpdateMessage), m_bufferSize);
	}
	else if (message->type == MessageType::Disconnect)
	{
		m_socket.send(message, sizeof(Message));
	}
	m_lock.unlock();
}

NetworkClient::~NetworkClient()
{
	Message m;
	m.type = MessageType::Disconnect;
	m.ID = m_ID;
	m_socket.send(&m, sizeof(Message), m_bufferSize);
	m_socket.setBlocking(false);
	m_socket.disconnect();
	m_connected = false;
	m_readerThread.join();
}

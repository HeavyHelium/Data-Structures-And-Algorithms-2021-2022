#ifndef  IMPLEMENTATION_HPP
#define IMPLEMENTATION_HPP

#if 1
#define TESTS
#endif

#if 0
#define CONSOLE
#endif

#include <iostream>
#include "interface.h"
#include "queue.hpp"
#include "list.hpp"
#include "list_with_sorted_insertion.hpp"

/// @brief same as Client, with the only exception that it stores an id and max departure minute
/// added partial order operators because they are used for determining priority
struct MyClient: Client 
{
	int MaxDepartureMinute;
	const int id;
	MyClient(const Client& cl, const int id)
		: Client{ cl.arriveMinute, cl.banana, cl.schweppes, cl.maxWaitTime }
		  , MaxDepartureMinute(arriveMinute + maxWaitTime), id(id)
	{}
	bool operator< (const MyClient& other) const { return MaxDepartureMinute < other.MaxDepartureMinute; }
	bool operator> (const MyClient& other) const { return other < *this; }
	bool operator>=(const MyClient& other) const { return !(*this < other); }
	bool operator<=(const MyClient& other) const { return other >= *this; }
	bool operator==(const MyClient& other) const { return MaxDepartureMinute == other.MaxDepartureMinute; }
	bool operator!=(const MyClient& other) const { return !(*this == other); }
	friend std::ostream& operator<<(std::ostream& os, const MyClient& c)
	{
		return os << c.arriveMinute << " " << c.banana << " " 
					<< c.schweppes << " " << c.maxWaitTime;
	}
};

struct client_remaining;

/// @brief keeps iterator to client list which is ascending order of max departure minute
/// it also keeps iterator to the respective element in the list, 
/// which stores iterators to remaining clients in order of arrival 
struct client_waiting
{
	typename list<MyClient>::iterator client_iter = list<MyClient>::iterator(nullptr);
	typename list<client_remaining>::iterator remaining_iter = list<client_remaining>::iterator(nullptr);
	bool operator< (const client_waiting& other) const { return *(client_iter) < *(other.client_iter); }
	bool operator> (const client_waiting& other) const { return other < *this; }
	bool operator>=(const client_waiting& other) const { return !(*this < other); }
	bool operator<=(const client_waiting& other) const { return other >= *this; }
	bool operator==(const client_waiting& other) const { return *client_iter == *other.client_iter; }
	bool operator!=(const client_waiting& other) const { return !(*this == other); }
};

/// @brief this structure is used when operating with the clients remaining in order of arrival
/// instead of storing the resource twice, it contains an iterator to the full clients list
/// it also has an iterator to the list keeping iterators to clients in ascending order of their max departure minute   
struct client_remaining
{
	typename list<MyClient>::iterator client_iter = list<MyClient>::iterator(nullptr);
	typename list_with_sorted_insertion<client_waiting>::iterator waiting_iter = list<client_waiting>::iterator(nullptr);
};

struct MyStore : Store 
{	
	static constexpr int RESTOCK_TIME = 60;
	static constexpr int RESTOCK_AMOUNT = 100;
	
	ActionHandler* actionHandler = nullptr;

	virtual void setActionHandler(ActionHandler* handler) override;
	virtual void init(int workerCount, int startBanana, int startSchweppes) override;

	virtual void addClients(const Client* clients, int count) override;

	virtual int getBanana() const override;
	virtual int getSchweppes() const override;
	virtual void advanceTo(int minute) override;
	void executeDay();
private:
	struct resource_state
	{
		int in_stock = 0; int arriving = 0;
		//int to_be_taken = 0;
		int estimate() const { return in_stock + arriving /* - to_be_taken*/; }
	};
	struct resources_state
	{
		resource_state banana;
		resource_state schweppes;
	}resources;
	struct arriving_resource 
	{ 
		ResourceType type; 
		const int delivery_minute; 
		arriving_resource(ResourceType t, int sendMinute)
			: type(t), delivery_minute(sendMinute + RESTOCK_TIME)
		{
#ifdef CONSOLE
		std::cout << "W " << sendMinute;
			switch (t)
			{
				case ResourceType::banana:
				std::cout << " banana\n"; break;
				case ResourceType::schweppes:
				std::cout << " schweppes\n"; break;
			}
#endif			
		}
	};
	struct workers_state { int total_cnt = 0; int available = 0; } workers;
	int last_client_id = 0;
	
	queue<arriving_resource> arriving_resources;//< to store arriving resources
	list<MyClient> clients;//< list of all clients who visited the store
	/// @brief insertion and deletion(as used) -- O(1) 
	list<client_remaining> clients_in_order;//< iterators to remaining clients in order of arrival, i.e. id
	/// @brief unsertion is O(n) and deletion is 0(1)(beacause it is used only with an iterator)
	list_with_sorted_insertion<client_waiting> clients_waiting;//< iterators to remaining clients in order of maxDepartureMinute

	/// @brief advances to arrive minute of client and takes action of sending workers when needed
	/// and serivicing the client when possible(or sends them to wait)
	/// @param iter iterator to the clients list
	void addClient(list<MyClient>::iterator iter);
	/// @brief sends worker for resource at a given minute
	/// @param T 
	/// @param minute 
	void sendWorker(ResourceType T, int minute);
	/// @brief makes a delivery of arriving resourcess
	void delivery();
	/// @brief makes a client departure at a given minute
	/// @param client 
	/// @param minute 
	void clientDeparture(const MyClient& client, int minute);
	/// @brief if amount is <= amount in stock, then amount units are taken of the resource, else the whole resorce in stock 
	/// @param T ResourceType
	/// @param amount int 
	/// @return int
	int takeResource(ResourceType T, int amount);
	/// @brief adds client to waiting lists
	/// @param iter iterator to the corresponding element in clients
	void sendClientToWait(typename list<MyClient>::iterator iter);
	/// @brief releases all clients before minute(exclusively), i.e. who have max departure times < minute
	/// @param minute int
	void releaseClientsBefore(int minute);
	/// @brief determines whether or not a client can be serviced right away
	/// @param c MyClient
	/// @return bool
	bool canBeServiced(const MyClient& c) const;
	/// @brief takes all actions from now to up until that minute(with that minute included)
	/// not to be confused with advanceTo since action does not add clients, compared to the former
	/// @param minute int
	void action(int minute);
	list<MyClient>::iterator iter = nullptr;//< iterator to newly added clients
	int max_departure_minute = 0;
public:
	static int calculate_workers_needed(int amount);
};

#endif // ! IMPLEMENTATION_HPP

#ifndef  IMPLEMENTATION_HPP
#define IMPLEMENTATION_HPP
#include <iostream>
#include "interface.h"
#include "queue.hpp"
#include "list.hpp"
#include "list_with_sorted_insertion.hpp"

/// @brief same as Client, with the only exception that it stores an id and max departure minute
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
	virtual void advanceTo(int minute) override;

	virtual int getBanana() const override;
	virtual int getSchweppes() const override;

private:
	struct resource_state
	{
		int in_stock = 0; int arriving = 0;
		int to_be_taken = 0;
		int estimate() const { return in_stock + arriving - to_be_taken; }
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
			///std::cout << "W " << sendMinute;
			switch (t)
			{
				case ResourceType::banana:
					/**std::cout << " banana\n"; */break;
				case ResourceType::schweppes:
					/**std::cout << " schweppes\n"*/; break;
			}
		}
	};
	struct workers_state 
	{ int total_cnt = 0; int available = 0; } workers;
	
	int last_client_id = 0;
	
	queue<arriving_resource> arriving_resources;//< to store arriving resources
	list<client_remaining> clients_in_order;//< iterators to remaining clients in order of arrival, i.e. id
	list_with_sorted_insertion<client_waiting> clients_waiting;//< iterators to remaining clients in order of max_departure_time

	list<MyClient> clients;//< clients sorted by max departure time

	void addClient(const MyClient& client);
	void sendWorker(ResourceType T, int minute);
	
	void delivery();
	void clientDeparture(const MyClient& client, int minute);
	int takeResource(ResourceType T, int amount);
	void sendClientToWait(typename list<MyClient>::iterator iter);
	void releaseClientsBefore(int minute);
	bool canBeServised(const MyClient& c) const;
	void addClientsAction();
public:
	static int calculate_workers_needed(int amount);
};

#endif // ! IMPLEMENTATION_HPP

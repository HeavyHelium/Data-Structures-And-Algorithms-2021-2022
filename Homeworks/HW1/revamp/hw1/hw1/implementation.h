#pragma once
#ifndef IMPLEMENTATION_H
#define IMPLEMENTATION_H

#include <iostream>
#include "interface.h"
#include "queue.h"
#include "list.h"
#include "list_with_sorted_insertion.h"

/// @brief same as Client, with the only exception that it stores an id and max departure minute
/// added partial order operators because they are used for determining priority
struct MyClient : Client
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

/// @brief keeps an iterator to client list which is in ascending order of max departure minute
/// it also keeps an iterator to the respective element in the list, 
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

	virtual void setActionHandler(ActionHandler* handler);
	virtual void init(int workerCount, int startBanana, int startSchweppes) override;

	virtual void addClients(const Client* clients, int count) override;

	virtual int getBanana() const override;
	virtual int getSchweppes() const override;
	virtual void advanceTo(int minute) override;
	/// @brief executes all events in a day
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
	int last_client_id = 0;//< used to track client ids

	queue<arriving_resource> arriving_resources;//< to store arriving resources
	list<MyClient> clients;//< list of all clients who visited the store
	/// iterators to remaining clients in order of arrival, i.e. id
	/// no data is copied, only iterators are stored
	list<client_remaining> clients_in_order;

	/// insertion is O(n) and deletion is 0(1)(beacause it is used only with an iterator)
	/// iterators to remaining clients in order of maxDepartureMinute
	list_with_sorted_insertion<client_waiting> clients_waiting;

	/// @brief sends worker for resource at a given minute
	/// @param T ResourceType
	/// @param minute int
	void sendWorker(ResourceType T, int minute);

	/// @brief makes a delivery of arriving resourcess
	void delivery(int minute);
	/// @brief client departure at a given minute
	void clientDeparture(const MyClient& client, int minute);
	/// @brief if amount is <= amount in stock, then amount units are taken of the resource, else the whole resorce in stock 
	int takeResource(ResourceType T, int amount);
	/// @brief adds client to waiting lists
	/// @param iter iterator to the corresponding element in clients
	void sendClientToWait(typename list<MyClient>::iterator iter);
	/// @brief releases all clients before minute(exclusively), i.e. who have max departure times < minute
	/// @param minute int
	/// @brief determines whether or not a client can be serviced right away
	/// @param c MyClient
	/// @return bool
	bool canBeServiced(const MyClient& c) const;
	/// @brief takes all actions from now to up until that minute(with that minute included)
	/// not to be confused with advanceTo since action does not add clients, compared to the former
	/// @param minute int
	void actionBefore(int minute);
	/// @brief at a given minute does the following:
	/// welcomes clients -- sends workers for banana, workers for schweppes(in this order),
	/// then delivery of resources occurs,
	/// then servicing of clients who need to go or can go because they are satisfied occurs(in order of arrival) 
	/// @param minute int 
	void actionAt(int minute);
	void releaseClientsByOrderBefore(int minute);
	void releaseClientsByNecessityBefore(int minute);
	list<MyClient>::iterator iter = nullptr;//< iterator to newly added clients
	int max_departure_minute = 0;
};

//-----------------------------------Definitions-----------------------------------
#include <cassert>
#include <cmath>

void MyStore::setActionHandler(ActionHandler* handler) { actionHandler = handler; }
void MyStore::init(int workerCount, int startBanana, int startSchweppes)
{
	workers.available = workers.total_cnt = workerCount;
	resources.banana.in_stock = startBanana;
	resources.schweppes.in_stock = startSchweppes;
}

void MyStore::addClients(const Client* clients, int count)
{
	if (count == 0) return;
	if (iter == this->clients.end())
		iter = this->clients.push_back(MyClient(clients[0], last_client_id++));
	if (max_departure_minute < iter->MaxDepartureMinute)
		max_departure_minute = iter->MaxDepartureMinute;
	for (int i = 1; i < count; ++i)
	{
		this->clients.push_back(MyClient(clients[i], last_client_id++));
		if (max_departure_minute < this->clients.back().MaxDepartureMinute)
			max_departure_minute = this->clients.back().MaxDepartureMinute;
	}

}

void MyStore::executeDay()
{
	assert(!clients.empty());
	advanceTo(INT_MAX);
	assert(arriving_resources.empty() && clients_waiting.empty()
		&& "All events must be completed at the end of the day ");
}

void MyStore::advanceTo(int minute)
{
	for (; iter != clients.end() && iter->arriveMinute <= minute;)
	{
		/// this changes iter
		if(iter->arriveMinute)
			actionBefore(iter->arriveMinute - 1);
		actionAt(iter->arriveMinute);
	}
	actionBefore(minute);
}

void MyStore::actionAt(int minute)
{
	// assumes current time is minute - 1, if minute is positive
	list<list<MyClient>::iterator> to_be_welcomed;
	
	while (iter != clients.end() && iter->arriveMinute == minute)
		to_be_welcomed.push_back(iter++);
	int sent_for_banana = 0;
	int sent_for_schweppes = 0;
	for (auto& elem : to_be_welcomed)
	{
		int banana_order = 0;
		int schweppes_order = 0;
		int banana_balance = resources.banana.estimate();
		int schweppes_balance = resources.schweppes.estimate();
		if (elem->banana > banana_balance) banana_order = elem->banana - banana_balance;
		if (elem->schweppes > schweppes_balance) schweppes_order = elem->schweppes - schweppes_balance;
		while ((banana_order > 0 || schweppes_order > 0) && workers.available)
		{
			if (banana_order >= schweppes_order)
			{
				sent_for_banana += 1;
				resources.banana.arriving += RESTOCK_AMOUNT;
				banana_order -= RESTOCK_AMOUNT;
			}
			else
			{
				sent_for_schweppes += 1;
				resources.schweppes.arriving += RESTOCK_AMOUNT;
				schweppes_order -= RESTOCK_AMOUNT;
			}
			--workers.available;
		}
		while (sent_for_banana) {
			sendWorker(ResourceType::banana, minute); 
			--sent_for_banana;
		}
		while (sent_for_schweppes) {
			sendWorker(ResourceType::schweppes, minute);
			--sent_for_schweppes;
		}
	}

	if(!arriving_resources.empty()
		&& arriving_resources.front().delivery_minute == minute)//deliver resources arriving at minute
		delivery(minute);

	releaseClientsByOrderBefore(minute);// to remove those who need to go or can go && were there before
	
	for (auto& elem : to_be_welcomed)
		if (canBeServiced(*elem)) clientDeparture(*elem, minute);
		else sendClientToWait(elem);
	releaseClientsByNecessityBefore(minute); // in case someone waits 0 minutes
	
}

void MyStore::actionBefore(int minute)
{
	// if no resources are arriving before or at minute and there are people waiting,
	// they need to go, in order of necessity
	if (!arriving_resources.empty() && arriving_resources.front().delivery_minute > minute
		|| arriving_resources.empty())
		releaseClientsByNecessityBefore(minute);
	// otherwise, we check if there are people who need to go by necessity before delivery,
	// we release those(in order of necessity), make the delivery,
	// then service people who can go because they are satisfied, or need to go because of necessity
	// hence we do that in order of arrival
	else
	{
		while (!arriving_resources.empty()
			&& arriving_resources.front().delivery_minute <= minute)
		{
			int delivery_time = arriving_resources.front().delivery_minute;
			releaseClientsByNecessityBefore(delivery_time - 1);
			delivery(delivery_time);
			releaseClientsByOrderBefore(delivery_time);
		}
		//releaseClientsByNecessityBefore(minute);// if after delivery someone has to go
	}
}

void MyStore::releaseClientsByOrderBefore(int minute)
{
	for (list<client_remaining>::iterator i = clients_in_order.begin();
		i != clients_in_order.end();)
	{
		list<client_remaining>::iterator j = i;
		++j;
		const MyClient& c = *(i->client_iter);
		if (canBeServiced(c) || c.MaxDepartureMinute <= minute)
		{
			clientDeparture(c, minute);
			assert(clients_waiting.size() == clients_in_order.size());
			typename list<client_waiting>::iterator ref = i->waiting_iter;
			clients_waiting.erase(ref);
			clients_in_order.erase(i);
		}
		i = j;// beacause of iterator invalidation, we have to use j
	}
}

void MyStore::releaseClientsByNecessityBefore(int minute)
{
	while (!clients_waiting.empty()
		&& clients_waiting.front().client_iter->MaxDepartureMinute <= minute)
	{
		client_waiting& f = clients_waiting.front();
		typename list<client_remaining>::iterator ref = f.remaining_iter;
		MyClient& current = *f.client_iter;
		clientDeparture(current, current.MaxDepartureMinute);
		clients_waiting.pop_front();
		clients_in_order.erase(ref);
	}
}

int MyStore::getBanana() const { return resources.banana.in_stock; }

int MyStore::getSchweppes() const { return resources.schweppes.in_stock; }

void MyStore::sendClientToWait(typename list<MyClient>::iterator iter)
{
	assert(!clients.empty());
	const MyClient& client = clients.back();
	typename list_with_sorted_insertion<client_waiting>::iterator
		waiting_iter = clients_waiting.insert(client_waiting{ iter });
	typename list<client_remaining>::iterator
		remaining_iter = clients_in_order.push_back(client_remaining{ iter, waiting_iter });
	waiting_iter->remaining_iter = remaining_iter;
}

void MyStore::sendWorker(ResourceType t, int minute)
{
#ifdef TESTS
	actionHandler->onWorkerSend(minute, t);
#endif


	arriving_resources.push(arriving_resource(t, minute));
}

/// @brief pops resource off the resources queue and updates states
void MyStore::delivery(int minute)
{
	assert(!arriving_resources.empty() && "No deliveries when queue of arrivng resources is empty.");
	while (!arriving_resources.empty()
		&& arriving_resources.front().delivery_minute == minute)
	{
		const arriving_resource& resource = arriving_resources.front();
#ifdef CONSOLE
		std::cout << "D " << resource.delivery_minute << " ";
#endif
#ifdef TESTS 
		actionHandler->onWorkerBack(resource.delivery_minute, resource.type);
#endif
		switch (resource.type)
		{
		case ResourceType::banana:
#ifdef CONSOLE 
			std::cout << "banana\n";
#endif 
			resources.banana.arriving -= RESTOCK_AMOUNT;
			resources.banana.in_stock += RESTOCK_AMOUNT;
			break;
		case ResourceType::schweppes:
#ifdef CONSOLE
			std::cout << "schweppes\n";
#endif
			resources.schweppes.arriving -= RESTOCK_AMOUNT;
			resources.schweppes.in_stock += RESTOCK_AMOUNT;
			break;
		}
		arriving_resources.pop();
		++workers.available;
	}
}

/// @brief 
/// @param client 
/// @param minute 
void MyStore::clientDeparture(const MyClient& client, int minute)
{
	int banana_taken = takeResource(ResourceType::banana, client.banana);
	int schweppes_taken = takeResource(ResourceType::schweppes, client.schweppes);
#ifdef TESTS
	actionHandler->onClientDepart(client.id, minute, banana_taken, schweppes_taken);
#endif
#ifdef CONSOLE
	std::cout << client.id << " " << minute << " "
		<< banana_taken << " " << schweppes_taken << " " << std::endl;
#endif
}
/// @brief if amount is smaller than or equal to the resource in stock, takes amount units of resource, 
/// otherwise takes what is in stock
/// @param T ResourceType
/// @param amount int
/// @return int
int MyStore::takeResource(ResourceType T, int amount)
{
	switch (T)
	{
		case ResourceType::banana:
		{
			int banana_taken = 0;
			if (amount > resources.banana.in_stock)
			{
				banana_taken = resources.banana.in_stock;
				resources.banana.in_stock = 0;
			}
			else { banana_taken = amount; resources.banana.in_stock -= banana_taken; }
			return banana_taken;
		}
		case ResourceType::schweppes:
		{
			int schweppes_taken = 0;
			if (amount > resources.schweppes.in_stock)
			{
				schweppes_taken = resources.schweppes.in_stock;
				resources.schweppes.in_stock = 0;
			}
			else { schweppes_taken = amount; resources.schweppes.in_stock -= schweppes_taken; }
			return schweppes_taken;
		}

	}
}

bool MyStore::canBeServiced(const MyClient& c) const
{
	return c.banana <= resources.banana.in_stock
		&& c.schweppes <= resources.schweppes.in_stock;
}

Store* createStore() { return new MyStore(); }

#endif // ! IMPLEMENTATION_HPP

//void MyStore::addClient(list<MyClient>::iterator iter)
//{
//	const MyClient& client = *iter;
//	if (client.arriveMinute) action(client.arriveMinute - 1);
//	if (client.banana <= resources.banana.in_stock
//		&& client.schweppes <= resources.schweppes.in_stock)
//	{
//		action(client.arriveMinute);
//		clientDeparture(client, client.arriveMinute);
//	}
//	else
//	{
//		int banana_order = 0;
//		int schweppes_order = 0;
//		int banana_balance = resources.banana.estimate();
//		int schweppes_balance = resources.schweppes.estimate();
//		if (client.banana > banana_balance) banana_order = client.banana - banana_balance;
//		if (client.schweppes > schweppes_balance) schweppes_order = client.schweppes - schweppes_balance;
//		//std::cout << "orders<b><s>: " << banana_order << ", " << schweppes_order << std::endl;
//		int workers_for_banana = calculate_workers_needed(banana_order);
//		int workers_for_schweppes = calculate_workers_needed(schweppes_order);
//		for (int i = 0; (i < workers_for_banana && workers.available); ++i, --workers.available)
//			sendWorker(ResourceType::banana, client.arriveMinute);
//		for (int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
//			sendWorker(ResourceType::schweppes, client.arriveMinute);
//
//		sendClientToWait(iter);
//		action(client.arriveMinute);
//	}
//}


//queue<int> sent_for_schweppes;
//for (auto& elem : to_be_welcomed)
//{
//	int banana_order = 0;
//	int banana_balance = resources.banana.estimate();
//	if (elem->banana > banana_balance) banana_order = elem->banana - banana_balance;
//	int workers_for_banana = calculate_workers_needed(banana_order);
//	for (int i = 0; (i < workers_for_banana && workers.available > 1); ++i, --workers.available)
//	{
//		sendWorker(ResourceType::banana, elem->arriveMinute);
//		banana_order -= RESTOCK_AMOUNT;
//	}
//}
//for (auto& elem : to_be_welcomed)
//{
//	int schweppes_order = 0;
//	int schweppes = resources.schweppes.estimate();
//	if (elem->schweppes > schweppes) schweppes_order = elem->schweppes - schweppes;
//	int workers_for_schweppes = calculate_workers_needed(schweppes_order);
//	for (int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
//		sendWorker(ResourceType::schweppes, elem->arriveMinute);
//}

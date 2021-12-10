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

	virtual void setActionHandler(ActionHandler* handler);
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
	void actionBefore(int minute);
	void actionAt(int minute);
	list<MyClient>::iterator iter = nullptr;//< iterator to newly added clients
	int max_departure_minute = 0;
public:
	static int calculate_workers_needed(int amount);
};


//------------------------------Implementation---------------------------
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
	//std::cout << max_departure_minute << std::endl;
	assert(!clients.empty());
	advanceTo(max_departure_minute);
	while (!arriving_resources.empty())
		delivery();
	assert(arriving_resources.empty() && clients_waiting.empty()
		&& "All events must be completed at the end of the day ");
}

void MyStore::advanceTo(int minute)
{
	// so time is minute
	//if (minute) actionBefore(minute - 1);

	for (; iter != clients.end() && iter->arriveMinute <= minute;)
	{
		/// this changes iter
		if(iter->arriveMinute)
			actionBefore(iter->arriveMinute - 1);
		actionAt(iter->arriveMinute);
		//std::cout << "here\n\n";
	}
	actionBefore(minute);
}

/// send for banana
/// send for schweppes
/// se
void MyStore::actionAt(int minute)
{
	// assumes current time is minute - 1, if minute is positive
	list<list<MyClient>::iterator> to_be_welcomed;
	
	while (iter != clients.end() && iter->arriveMinute == minute)
		to_be_welcomed.push_back(iter++);

	for (auto& elem : to_be_welcomed)
	{
		int banana_order = 0;
		int banana_balance = resources.banana.estimate();
		if (elem->banana > banana_balance) banana_order = elem->banana - banana_balance;
		//std::cout << "orders<b><s>: " << banana_order << ", " << schweppes_order << std::endl;
		int workers_for_banana = calculate_workers_needed(banana_order);
		for (int i = 0; (i < workers_for_banana && workers.available); ++i, --workers.available)
			sendWorker(ResourceType::banana, elem->arriveMinute);
	}
	for (auto& elem : to_be_welcomed)
	{
		int schweppes_order = 0;
		int schweppes = resources.schweppes.estimate();
		if (elem->schweppes > schweppes) schweppes_order = elem->schweppes - schweppes;
		//std::cout << "orders<b><s>: " << schweppes_order << ", " << schweppes_order << std::endl;
		int workers_for_schweppes = calculate_workers_needed(schweppes_order);
		for (int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
			sendWorker(ResourceType::schweppes, elem->arriveMinute);
	}
	while (!arriving_resources.empty()
		&& arriving_resources.front().delivery_minute == minute)
		delivery();
	//std::cout << "rstate: " << resources.banana.estimate() << ", " << resources.schweppes.estimate() << "\n";
	for (auto& elem : to_be_welcomed)
		if (canBeServiced(*elem))
		{
			///*std::cout << "hhh \n"*/; 
			clientDeparture(*elem, minute);
		}
		else {
			///*std::cout << "ccc\n";*/  std::cout << minute << ", ";
			sendClientToWait(elem);
		}
	actionBefore(minute);

}

void MyStore::actionBefore(int minute)
{
	//std::cout << minute << " there\n\n";
	//std::cout << "hakunamatata " << arriving_resources.size() << ", " << clients_waiting.size() << "\n";
	if (!arriving_resources.empty() && arriving_resources.front().delivery_minute > minute
		|| arriving_resources.empty())
	{
		//std::cout << "there2\n\n";  
		releaseClientsBefore(minute + 1);
	}
	else
	{
		//std::cout << "wtf " << arriving_resources.size() << ", " << clients_waiting.size() << "\n\n";
		/// TODO refactor!!!
		while (!arriving_resources.empty()
			&& arriving_resources.front().delivery_minute <= minute)
		{
			int delivery_time = arriving_resources.front().delivery_minute;
			releaseClientsBefore(delivery_time);

			while (!arriving_resources.empty()
				&& arriving_resources.front().delivery_minute == delivery_time)
				delivery();

			for (list<client_remaining>::iterator i = clients_in_order.begin();
				i != clients_in_order.end();)
			{
				list<client_remaining>::iterator j = i;
				++j;
				const MyClient& c = *(i->client_iter);
				if (canBeServiced(c) || c.MaxDepartureMinute == delivery_time)
				{
					clientDeparture(c, delivery_time);
					assert(clients_waiting.size() == clients_in_order.size());
					typename list<client_waiting>::iterator ref = i->waiting_iter;
					clients_waiting.erase(ref);
					//std::cout << "after first\n\n";
					clients_in_order.erase(i);
				}
				i = j;// beacause of iterator invalidation, we have to use j
			}


		}
		releaseClientsBefore(minute + 1);// if after delivery someone has to go
	}

	/*	actionHandler->onWorkerSend(0, ResourceType::banana);
		actionHandler->onWorkerBack(0, ResourceType::schweppes);
		actionHandler->onClientDepart(0, 0, 1, 2);*/
}

void MyStore::releaseClientsBefore(int minute)
{
	while (!clients_waiting.empty()
		&& clients_waiting.front().client_iter->MaxDepartureMinute < minute)
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

/// @brief helper function to calculate the workers needed
/// @param amount int
/// @return int
int MyStore::calculate_workers_needed(int amount)
{
	assert("amount should be non-negative" && amount >= 0);
	int needed = std::ceil((double)amount / RESTOCK_AMOUNT);
	return needed;
}

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

void MyStore::sendClientToWait(typename list<MyClient>::iterator iter)
{
//std::cout << "sent to wait, arrived:  " << iter->arriveMinute << "\n";
	assert(!clients.empty());
	const MyClient& client = clients.back();
	typename list_with_sorted_insertion<client_waiting>::iterator
		waiting_iter = clients_waiting.insert(client_waiting{ iter });
	typename list<client_remaining>::iterator
		remaining_iter = clients_in_order.push_back(client_remaining{ iter, waiting_iter });
	waiting_iter->remaining_iter = remaining_iter;
}

/// @brief adds resource to resources queue and updates states
/// @param t ResourseType
/// @param minute int
void MyStore::sendWorker(ResourceType t, int minute)
{
#ifdef TESTS
	actionHandler->onWorkerSend(minute, t);
#endif

	arriving_resources.push(arriving_resource(t, minute));
	switch (t)
	{
	case ResourceType::banana: resources.banana.arriving += RESTOCK_AMOUNT; break;
	case ResourceType::schweppes: resources.schweppes.arriving += RESTOCK_AMOUNT; break;
	}
	//--workers.available;
}

/// @brief pops resource off the resources queue and updates states
void MyStore::delivery()
{
	assert(!arriving_resources.empty() && "No deliveries when queue of arrivng resources is empty.");
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
	//std::cout << "goes: ";
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
	/// TODO: make template and avoid code duplicaion
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
		//resources.banana.to_be_taken -= banana_taken;
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
		//resources.schweppes.to_be_taken -= schweppes_taken;
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



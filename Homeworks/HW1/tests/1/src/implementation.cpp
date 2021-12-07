#include "implementation.hpp"
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
	assert(count >= 1);
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
}
/*
action(before_client_comes); addClients up to minute
*/

void MyStore::advanceTo(int minute)
{
	if (clients.empty() || (!clients.empty() && iter->arriveMinute > minute))
	{ action(minute); return; }
	
	for (; iter != clients.end() && iter->arriveMinute <= minute; ++iter)
		addClient(iter);
//std::cout << "here\n\n";
	action(minute);
}

void MyStore::action(int minute) 
{
//std::cout << minute << " there\n\n";
//std::cout << "hakunamatata " << arriving_resources.size() << ", " << clients_waiting.size() << "\n\n";
	if (!arriving_resources.empty() && arriving_resources.front().delivery_minute > minute
		|| arriving_resources.empty())
	{
//std::cout << "there2\n\n";  
		releaseClientsBefore(minute + 1);
	}
	else
	{
//std::cout << "wtf " << arriving_resources.size() << ", " << clients_waiting.size() << "\n\n";
		while (!arriving_resources.empty() 
			&& arriving_resources.front().delivery_minute <= minute)
		{
			int delivery_time = arriving_resources.front().delivery_minute;
			releaseClientsBefore(delivery_time);
			while (!arriving_resources.empty()
				&& arriving_resources.front().delivery_minute == delivery_time)
			{
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

					i = j;// beacause of iterator invalidation
				}
			}


		}
		releaseClientsBefore(minute + 1);
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

void MyStore::addClient(list<MyClient>::iterator iter)
{
	const MyClient& client = *iter;
	action(client.arriveMinute);
	if (client.banana <= resources.banana.in_stock
		&& client.schweppes <= resources.schweppes.in_stock)
	{
		resources.banana.to_be_taken += client.banana;
		resources.schweppes.to_be_taken += client.schweppes;
		clientDeparture(client, client.arriveMinute);
	}
	else
	{
		int banana_order = 0;
		int schweppes_order = 0;
		int banana_balance = resources.banana.estimate();
		int schweppes_balance = resources.schweppes.estimate();
		if (client.banana > banana_balance) banana_order = client.banana - banana_balance;
		if (client.schweppes > schweppes_balance) schweppes_order = client.schweppes - schweppes_balance;
//std::cout << "orders<b><s>: " << banana_order << ", " << schweppes_order << std::endl;
		int workers_for_banana = calculate_workers_needed(banana_order);
		int workers_for_schweppes = calculate_workers_needed(schweppes_order);
		for (int i = 0; (i < workers_for_banana && workers.available); ++i, --workers.available)
			sendWorker(ResourceType::banana, client.arriveMinute);
		for (int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
			sendWorker(ResourceType::schweppes, client.arriveMinute);
//std::cout << "\n\n" << resources.banana.estimate() << ", " << resources.schweppes.estimate() << "\n\n";
		sendClientToWait(iter);
		resources.banana.to_be_taken += client.banana;
		resources.schweppes.to_be_taken += client.schweppes;
	}
}

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
			resources.banana.to_be_taken -= banana_taken;
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
			resources.schweppes.to_be_taken -= schweppes_taken;
			return schweppes_taken;
		}

	}
}

bool MyStore::canBeServiced(const MyClient& c) const
{
	return c.banana <= resources.banana.in_stock 
		&& c.schweppes <= resources.schweppes.in_stock;
}

Store *createStore() { return new MyStore(); }
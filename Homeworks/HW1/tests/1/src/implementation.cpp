#include "interface.h"
#include <cstddef>
#include <vector>
#include <queue>
#include <iostream>
#include "priority_queue.hpp"


constexpr unsigned RESTOCK_TIME = 60;
constexpr unsigned RESTOCK_AMOUNT = 100;

struct my_client : public Client
{
	my_client(unsigned arriveMinute, unsigned banana, unsigned schweppes, unsigned maxWaitTime, unsigned id)
		: Client{ arriveMinute, banana, schweppes, maxWaitTime },
		id(id)
	{}
	unsigned max_departure_time()const
	{
		return arriveMinute + maxWaitTime;
	}
	bool operator<(const my_client& other) const
	{
		return max_departure_time() > other.max_departure_time();
	}
	unsigned get_id() const { return id; }
private:
	unsigned id;
};


struct MyStore : Store
{
	ActionHandler* actionHandler = nullptr;
	void setActionHandler(ActionHandler* handler) override
	
	{ actionHandler = handler; }

	void init(int workerCount, int startBanana, int startSchweppes) override
	{
		workers.total_cnt = workerCount;
		banana.in_stock = startBanana;
		schweppes.in_stock = startSchweppes;
	}

	void addClients(const Client* clients, int count) override
	{
		for (std::size_t i = 0; i < count; ++i)
			addClient();
	}

	void advanceTo(int minute) override
	{
		while ((!clients_waiting.empty()) 
				&& clients_waiting.top().max_departure_time() <= minute)
		{
			const my_client& current_client = clients_waiting.top();
			unsigned last_arrived_good = current_client.max_departure_time();
			bool done = false;
			while ((!arriving_goods.empty())
				&& arriving_goods.front().time < current_client.max_departure_time())
			{
				last_arrived_good = arriving_goods.front().time;
				switch(arriving_goods.front().type)
				{
				case arriving_good::T::banana: 
				{ banana.in_stock += RESTOCK_AMOUNT; banana.arriving -= RESTOCK_AMOUNT; }
				case arriving_good::T::schweppes: 
				{ schweppes.in_stock += RESTOCK_AMOUNT; schweppes.arriving -= RESTOCK_AMOUNT; }
				}
				arriving_goods.pop();
				if (banana.in_stock >= current_client.banana && schweppes.in_stock >= current_client.schweppes)
				{
					// ready to go
					std::cout << current_client.id << " " << last_arrived_good << " "
						<< current_client.banana << " " << current_client.schweppes << std::endl;
					banana.in_stock -= current_client.banana;
					schweppes.in_stock -= current_client.schweppes;
					clients_waiting.pop();
					done = true;
					break;
				}
			}
			if (done) continue;
			else
			{
				unsigned banana_to_take = current_client.banana > banana.in_stock ? banana.in_stock : current_client.banana;
				unsigned schweppes_to_take = current_client.schweppes > schweppes.in_stock ? schweppes.in_stock : current_client.schweppes;
				std::cout << current_client.id << " " << current_client.max_departure_time() << " "
					<< banana_to_take << " " << schweppes_to_take << std::endl;
				banana.in_stock -= banana_to_take;
				schweppes.in_stock -= schweppes_to_take;
				clients_waiting.pop();
			}

		}
	}

	virtual int getBanana() const
	{ return banana.in_stock; }

	virtual int getSchweppes() const
	{ return schweppes.in_stock; }

private:
	struct goods_state { unsigned in_stock = 0; unsigned arriving = 0; } banana, schweppes;
	struct workers_state { unsigned total_cnt = 0; unsigned available = 0; } workers;
	struct arriving_good { enum T { banana, schweppes }type; unsigned time;  };
	priority_queue<my_client> clients_waiting;
	std::queue<arriving_good> arriving_goods;// this -- make a queue
	unsigned last_client_id = 0;

	void addClient(const my_client& c)
	{
		if (c.arriveMinute) advanceTo(c.arriveMinute);
		if (c.banana > banana.in_stock || c.schweppes > schweppes.in_stock)
		{
			if(c.banana <= (banana.in_stock + banana.arriving))
			{
				clients_waiting.push(c);
			}
		}
		else
		{

		}

	};
};

Store* createStore() 
{ return new MyStore(); } 

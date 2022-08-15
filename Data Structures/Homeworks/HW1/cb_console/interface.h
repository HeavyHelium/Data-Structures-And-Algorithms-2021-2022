#ifndef INTERFACE_H
#define INTERFACE_H

#include "..\priority_queue\priority_queue.hpp"
#include "..\queue\queue.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
struct Client
{
	int arriveMinute;
	int banana;
	int schweppes;
	int maxWaitTime;
};

enum class ResourceType
{
	banana,
	schweppes,
};


constexpr int RESTOCK_TIME = 60;
constexpr int RESTOCK_AMOUNT = 100;

struct my_client : public Client
{
    my_client(int arriveMinute, int banana, int schweppes, int maxWaitTime, int id)
        :   Client{ arriveMinute, banana, schweppes, maxWaitTime },
            id(id)
    {}
	int max_departure_time()const
	{ return arriveMinute + maxWaitTime; }
	bool operator<(const my_client& other) const
	{ return max_departure_time() > other.max_departure_time(); }
	bool operator>=(const my_client& other) const
	{ return !(*this < other); }
	int get_id() const { return id; }
private:
	int id;
};


struct ActionHandler {

	virtual void onWorkerSend(int minute, ResourceType resource) = 0;

	virtual void onWorkerBack(int minute, ResourceType resource) = 0;

	virtual void onClientDepart(int index, int minute, int banana, int schweppes) = 0;
};

struct Store {
	virtual void init(int workerCount, int startBanana, int startSchweppes) = 0;

	virtual void setActionHandler(ActionHandler *handler) = 0;

	/// NOTE: Tests may call this method multiple times, this must append new clients to existing clients in the store
	virtual void addClients(const Client *clients, int count) = 0;

	/// Advance the time to the given minute. All events up to and *including* the minute must be generated
	/// Events that happen at a later time must *not* be generated
	/// NOTE: Multiple calls to the function will always pass increasing values
	virtual void advanceTo(int minute) = 0;

	/// Get the amount of bananas currently in the store
	virtual int getBanana() const = 0;

	/// Get the amount of schweppes currently in the store
	virtual int getSchweppes() const = 0;

	virtual ~Store() {}
};
Store *createStore();


struct MyStore : Store
{
	ActionHandler* actionHandler = nullptr;
	void setActionHandler(ActionHandler* handler) override

	{ actionHandler = handler; }

	void init(int workerCount, int startBanana, int startSchweppes) override
	{
		workers.available = workers.total_cnt = workerCount;
		banana.in_stock = startBanana;
		schweppes.in_stock = startSchweppes;
	}

	void advanceTo(int minute) override
	{
	    /// while there are products which arrive before minute
        while(!arriving_goods.empty() && arriving_goods.front().time > minute)
        {

        }

	}

	void pop_client()
	{
	    assert(clients_waiting.empty() == false);
	    goods_taken taken{0, 0};
        my_client& current = clients_waiting.top();
        if(current.banana > banana.in_stock)
        {
            taken.banana = banana.in_stock;
            banana.to_be_taken -= taken.banana;
            banana.in_stock = 0;
        }
        else
        {
            taken.banana = current.banana;
            banana.in_stock -= taken.banana;
        }
        if(current.schweppes > schweppes.in_stock)
        {
            taken.schweppes = schweppes.in_stock;
            schweppes.to_be_taken -= taken.schweppes;
            schweppes.in_stock = 0;
        }
        else
        {
            taken.schweppes = current.schweppes;
            schweppes.in_stock -= taken.schweppes;
        }
        banana.to_be_taken -= taken.banana;
        schweppes.to_be_taken -= taken.schweppes;
        std::cout << "goes: " << current.max_departure_time() << " " << current.get_id() << " "
                        << taken.banana << " " << taken.schweppes << " "  << std::endl;
        clients_waiting.pop();
	}

	void addClients(const Client* clients, int count) override
	{
		for (std::size_t i = 0; i < count; ++i)
        {
            addClient(clients[i]);
        }
	}
	void addClient(const Client& c)
	{
	    int id = last_client_id++;
		if (c.arriveMinute) advanceTo(c.arriveMinute);
		// if there are enough products, take them and go
		if(c.banana <= banana.in_stock && c.schweppes <= schweppes.in_stock)
        {
            //std::cout << "here\n\n";
            // <id> <departure> <banana> <schweppes>
            std::cout << "goes: " << c.arriveMinute << " " << id <<" " << c.banana << " " << c.schweppes  << std::endl;
            banana.in_stock -= c.banana;
            schweppes.in_stock -= c.schweppes;
        }
        // else if there is enough of them arriving, go wait
        else if(c.banana <= banana.total() && c.schweppes <= schweppes.total())
        {
            clients_waiting.push(my_client(c.arriveMinute, c.banana, c.schweppes, c.maxWaitTime, id));
            banana.to_be_taken += c.banana;
            schweppes.to_be_taken += c.schweppes;
        }
        // else we have to send a worker if available and send to wait
        else
        {
            //std::cout << "here!\n\n";
            int workers_for_banana = (c.banana > banana.total()) ? calculate_workers_needed(c.banana) : 0;
            int workers_for_schweppes = (c.schweppes > schweppes.total()) ? calculate_workers_needed(c.schweppes) : 0;
            //std::cout << workers.available << ",, " << workers_for_schweppes << std::endl;
            for(int i = 0; (i < workers_for_banana && workers.available); ++i, --workers.available)
            {
                arriving_goods.push(arriving_good{ arriving_good::T::banana, c.arriveMinute + RESTOCK_TIME });
                banana.arriving += RESTOCK_AMOUNT;
                std::cout << "W " << c.arriveMinute << " banana" << std::endl;
            }

            for(int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
            {
                arriving_goods.push(arriving_good{ arriving_good::T::schweppes, c.arriveMinute + RESTOCK_TIME });
                schweppes.arriving += RESTOCK_AMOUNT;
                std::cout << "W " << c.arriveMinute << " schweppes" << std::endl;
            }
            clients_waiting.push(my_client(c.arriveMinute, c.banana, c.schweppes, c.maxWaitTime, last_client_id++));
            banana.to_be_taken += c.banana;
            schweppes.to_be_taken += c.schweppes;
            //std::cout << clients_waiting.size() << ", " << arriving_goods.size();
            //; << ", wav: " << workers_state.available << std::endl;
        }


	};

	virtual int getBanana() const override { return banana.in_stock; }

	virtual int getSchweppes() const override { return schweppes.in_stock; }

private:
    static int calculate_workers_needed(int amount)
    {
        assert("amount should be positive" && amount > 0);
        int needed = std::ceil((double)amount / RESTOCK_AMOUNT);
        return needed;
    }
	struct goods_state
	{
	    int in_stock = 0; int arriving = 0;
	    int to_be_taken = 0;
        int total() const { return in_stock + arriving - to_be_taken; }
    } banana, schweppes;
    struct goods_taken { int banana; int schweppes; };
	struct workers_state { int total_cnt = 0; int available = 0; } workers;
	struct arriving_good { enum T { banana, schweppes }type; int time;  };
	priority_queue<my_client> clients_waiting;
	queue<arriving_good> arriving_goods;// this -- make a queue
	int last_client_id = 0;
};


#endif // INTERFACE_H




////while ((!clients_waiting.empty())
////        && clients_waiting.top().max_departure_time() <= minute)
////{
////    const my_client& current_client = clients_waiting.top();
////    int last_arrived_good = current_client.max_departure_time();
////    bool done = false;
////    while ((!arriving_goods.empty())
////        && arriving_goods.front().time < current_client.max_departure_time())
////    {
////        last_arrived_good = arriving_goods.front().time;
////        switch(arriving_goods.front().type)
////        {
////        case arriving_good::T::banana:
////        { banana.in_stock += RESTOCK_AMOUNT; banana.arriving -= RESTOCK_AMOUNT; }
////        case arriving_good::T::schweppes:
////        { schweppes.in_stock += RESTOCK_AMOUNT; schweppes.arriving -= RESTOCK_AMOUNT; }
////        }
////        arriving_goods.pop();
////        if (banana.in_stock >= current_client.banana && schweppes.in_stock >= current_client.schweppes)
////        {
////            // ready to go
////            std::cout << /*"goes: " << */current_client.get_id() << " " << last_arrived_good << " "
////                << current_client.banana << " " << current_client.schweppes << std::endl;
////            banana.in_stock -= current_client.banana;
////            schweppes.in_stock -= current_client.schweppes;
////            clients_waiting.pop();
////            done = true;
////            break;
////        }
////    }
////    if (done) continue;
////    else
////    {
////        int banana_to_take = current_client.banana > banana.in_stock ? banana.in_stock : current_client.banana;
////        int schweppes_to_take = current_client.schweppes > schweppes.in_stock ? schweppes.in_stock : current_client.schweppes;
////        std::cout << "goes: " << current_client.get_id() << " " << current_client.max_departure_time() << " "
////            << banana_to_take << " " << schweppes_to_take << std::endl;
////        banana.in_stock -= banana_to_take;
////        schweppes.in_stock -= schweppes_to_take;
////        clients_waiting.pop();
////    }
////
////}


//// while(!arriving_goods.empty() && arriving_goods.front().time <= first_departing)
//// {
////     const arriving_good& first_good = arriving_goods.front();
////     switch (first_good.type)
////     {
////         case arriving_good::T::banana :
////         {
////             banana.arriving -= RESTOCK_AMOUNT;
////             banana.in_stock += RESTOCK_AMOUNT;
////             break;
////         }
////         case arriving_good::T::schweppes :
////         {
////             schweppes.arriving -= RESTOCK_AMOUNT;
////             schweppes.in_stock += RESTOCK_AMOUNT;
////             break;
////         }
////     }
////     arriving_goods.pop();
//// }
//// // if there are enough thin
//// if()

//////	    if(clients_waiting.empty() && arriving_goods.empty()
//////            || !clients_waiting.empty() && arriving_goods.empty() && clients_waiting.top().max_departure_time() > minute
//////            || clients_waiting.empty() && !arriving_goods.empty() && arriving_goods.front().time > minute
//////            || clients_waiting.empty() && arriving_goods.empty()
//////            )
//////                return;
//////
//////	    if (clients_waiting.empty())
//////        {
//////            while(!arriving_goods.empty() && arriving_goods.front().time <= minute)
//////            {
//////                const arriving_good& first = arriving_goods.front();
//////                switch (first.type)
//////                {
//////                    case arriving_good::T::banana:
//////                    {
//////                        banana.in_stock += RESTOCK_AMOUNT;
//////                        banana.arriving -= RESTOCK_AMOUNT;
//////                        break;
//////                    }
//////                    case arriving_good::T::schweppes:
//////                    {
//////                        schweppes.in_stock += RESTOCK_AMOUNT;
//////                        schweppes.arriving -= RESTOCK_AMOUNT;
//////                        break;
//////                    }
//////                }
//////                arriving_goods.pop();
//////            }
//////
//////        }
//////        else if (arriving_goods.empty())
//////        {
//////            while(!clients_waiting.empty() && clients_waiting.top().max_departure_time() <= minute)
//////            {
//////                // take what you can and leave this damned place
//////                const my_client& current = clients_waiting.top();
//////                int taken_banana = 0;
//////                if(current.banana > banana.in_stock)
//////                { taken_banana = banana.in_stock; banana.in_stock = 0; }
//////                else { taken_banana = current.banana; banana.in_stock -= taken_banana; }
//////                int taken_schweppes = 0;
//////                if(current.schweppes > schweppes.in_stock)
//////                { taken_schweppes = schweppes.in_stock; schweppes.in_stock = 0; }
//////                else { taken_schweppes = current.schweppes; schweppes.in_stock -= taken_schweppes; }
//////                std::cout << "goes: " << current.arriveMinute << " " << current.get_id() << " " << taken_banana << " " << taken_schweppes << " "  << std::endl;
//////                clients_waiting.pop();
//////            }
//////        }
//////	    else if(!clients_waiting.empty() && !arriving_goods.empty())
//////        {
//////            int first_arriving_good = arriving_goods.front().time;
//////            int first_departing = clients_waiting.top().max_departure_time();
//////            int min = std::min(first_arriving_good, first_departing);
//////            if(minute < min) return;
//////            if(first_arriving_good < first_departing)
//////                { advanceTo(first_arriving_good); advanceTo(minute); }
//////            else if(first_arriving_good > first_departing)
//////                { advanceTo(first_departing); advanceTo(minute); }
//////            else
//////            {
//////                while(!arriving_goods.empty() && arriving_goods.front().time == first_departing)
//////                {
//////                    const arriving_good& first = arriving_goods.front();
//////                    switch(first.type)
//////                    {
//////                        case arriving_good::T::banana:
//////                        {
//////                            banana.in_stock += RESTOCK_AMOUNT;
//////                            banana.arriving -= RESTOCK_AMOUNT;
//////                            break;
//////                        }
//////                        case arriving_good::T::schweppes:
//////                        {
//////                            schweppes.in_stock += RESTOCK_AMOUNT;
//////                            schweppes.arriving -= RESTOCK_AMOUNT;
//////                            break;
//////                        }
//////                    }
//////                }
//////            const my_client& c = clients_waiting.top();
//////            if(c.banana <= banana.in_stock && c.schweppes <= schweppes.in_stock)
//////            {
//////                std::cout << "goes: " << c.arriveMinute << " " << c.get_id() << " " << c.schweppes << " " << c.banana << " "  << std::endl;
//////                banana.in_stock -= c.banana;
//////                schweppes.in_stock -= c.schweppes;
//////                clients_waiting.pop();
//////            }
//////            // else if there is enough of them arriving, keep waiting
//////            else if(c.banana <= banana.in_stock + banana.arriving && schweppes.arriving + schweppes.in_stock <= c.schweppes)
//////            {}
//////            // else we have to send a worker if available and keep waiting
//////            else
//////            {
//////                int workers_for_banana = (c.banana > banana.total()) ? calculate_workers_needed(c.banana) : 0;
//////                int workers_for_schweppes = (c.schweppes > schweppes.total()) ? calculate_workers_needed(c.schweppes) : 0;
//////                //std::cout << "here!!\n\n";
//////                //std::cout << "w_avb: " << workers.available << std::endl;
//////                //std::cout << workers_for_banana << ", " << workers_for_schweppes << std::endl << std::endl;
//////                /// TODO: make it constant
//////                // does this make it quadratic???
//////                for(int i = 0; (i < workers_for_banana && workers.available); ++i, --workers.available)
//////                {
//////                    /// TODO fix goods queue --> make insert possible at a random location
//////                    arriving_goods.push(arriving_good{ arriving_good::T::banana, c.arriveMinute + RESTOCK_TIME });
//////                    banana.arriving += RESTOCK_AMOUNT;
//////                    std::cout << "W " << c.arriveMinute << " banana" << std::endl;
//////                }
//////
//////                for(int i = 0; (i < workers_for_schweppes && workers.available); ++i, --workers.available)
//////                {
//////                    arriving_goods.push(arriving_good{ arriving_good::T::schweppes, c.arriveMinute + RESTOCK_TIME });
//////                    schweppes.arriving += RESTOCK_AMOUNT;
//////                    std::cout << "W " << c.arriveMinute << " schweppes" << std::endl;
//////                }
//////            }
//////
//////            }
//////
//////        }

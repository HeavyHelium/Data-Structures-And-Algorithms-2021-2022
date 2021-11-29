#pragma once


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

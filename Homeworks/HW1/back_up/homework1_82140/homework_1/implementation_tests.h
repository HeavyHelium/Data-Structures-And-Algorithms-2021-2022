#pragma once
#ifndef MYTESTS_HPP
#define MYTESTS_HPP

#include "catch.hpp"

TEST_CASE("Examples") {

	SECTION("first")
	{
		TestStore store;
		store.init(5, 0, 0);
		/*
		5 4
		0 50 50 70
		10 50 50 50
		60 50 50 0
		60 50 50 50*/

		store.addClients({
			Client{0, 50, 50, 70},
			Client{10, 50, 50, 50},
			Client{60, 50, 50, 0},
			Client{60, 50, 50, 50}
			});

		REQUIRE(store.getBanana() == 0);
		REQUIRE(store.getSchweppes() == 0);
		REQUIRE(store.log.empty());

		SECTION("Zeroth client arrival") {
			store.advanceTo(0);
			INFO("First client will trigger 2 workers");
			REQUIRE(store.log.size() == 2);
			REQUIRE(store.log[0].type == StoreEvent::WorkerSend);
			INFO("Workers are first sent for banana");
			REQUIRE(store.log[0].worker.resource == ResourceType::banana);
			REQUIRE(store.log[0].minute == 0);
			REQUIRE(store.log[1].type == StoreEvent::WorkerSend);
			REQUIRE(store.log[1].worker.resource == ResourceType::schweppes);
			REQUIRE(store.log[1].minute == 0);
		}

		SECTION("First client arrival") {

			INFO("Resources have not yet arrived");
			REQUIRE(store.getBanana() == 0);
			REQUIRE(store.getSchweppes() == 0);
			REQUIRE(store.log.empty());
			store.advanceTo(10);
			REQUIRE("No events should happen, no workers should be sent");
			REQUIRE(store.log.size() == 2);
		}

		SECTION("Zeroth, first and second depart") {
			store.advanceTo(60);
			REQUIRE(store.log.size() == 7);
			INFO("First client should be welcomed but no workers should be sent, same with second client");
			INFO("Then workers should arrive");
			REQUIRE(store.log[2].type == StoreEvent::WorkerBack);
			REQUIRE(store.log[2].worker.resource == ResourceType::banana);
			REQUIRE(store.log[3].type == StoreEvent::WorkerBack);
			REQUIRE(store.log[3].worker.resource == ResourceType::schweppes);

			INFO("Zeroth client and first client can both be serviced, but zeroth is serviced first");
			REQUIRE(store.log[4].type == StoreEvent::ClientDepart);
			REQUIRE(store.log[4].client.banana == 50);
			REQUIRE(store.log[4].client.schweppes == 50);
			REQUIRE(store.log[4].client.index == 0);
			REQUIRE(store.log[4].minute == 60);

			REQUIRE(store.log[5].type == StoreEvent::ClientDepart);
			REQUIRE(store.log[5].client.banana == 50);
			REQUIRE(store.log[5].client.schweppes == 50);
			REQUIRE(store.log[5].client.index == 1);
			REQUIRE(store.log[5].minute == 60);

			REQUIRE(store.getBanana() == 0);
			REQUIRE(store.getSchweppes() == 0);

			INFO("Second and third must be sent waiting and then depart with nothing");

			REQUIRE(store.log[6].type == StoreEvent::ClientDepart);
			REQUIRE(store.log[6].client.banana == 0);
			REQUIRE(store.log[6].client.schweppes == 0);
			REQUIRE(store.log[6].client.index == 2);
			REQUIRE(store.log[6].minute == 60);
		}
		SECTION("Third departs later")
		{
			store.advanceTo(110);
			REQUIRE(store.log.size() == 8);
			REQUIRE(store.log[7].type == StoreEvent::ClientDepart);
			REQUIRE(store.log[7].client.banana == 0);
			REQUIRE(store.log[7].client.schweppes == 0);
			REQUIRE(store.log[7].client.index == 3);
			REQUIRE(store.log[7].minute == 110);
			REQUIRE(store.getBanana() == 0);
			REQUIRE(store.getSchweppes() == 0);
		}

	}
	/*
	4 1
	0 350 200 60*/
	SECTION("second")
	{
		TestStore store1;
		store1.init(4, 0, 0);
		store1.addClients(Client{ 0, 350, 200, 60 });// 3 for banana, 1 for schweppes
		REQUIRE(store1.log.size() == 0);
		SECTION("Zeroth minute")
		{
			store1.advanceTo(0);
			REQUIRE(store1.log.size() == 4);
			REQUIRE(store1.log[0].type == StoreEvent::WorkerSend);
			REQUIRE(store1.log[0].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[0].minute == 0);
			REQUIRE(store1.log[1].type == StoreEvent::WorkerSend);
			REQUIRE(store1.log[1].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[1].minute == 0);
			REQUIRE(store1.log[2].type == StoreEvent::WorkerSend);
			REQUIRE(store1.log[2].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[2].minute == 0);
			REQUIRE(store1.log[3].type == StoreEvent::WorkerSend);
			REQUIRE(store1.log[3].worker.resource == ResourceType::schweppes);
			REQUIRE(store1.log[3].minute == 0);
		}
		SECTION("60th minute")
		{
			store1.advanceTo(60);// arrival of resources
			REQUIRE(store1.log.size() == 9);
			REQUIRE(store1.log[4].type == StoreEvent::WorkerBack);
			REQUIRE(store1.log[4].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[4].minute == 60);
			REQUIRE(store1.log[5].type == StoreEvent::WorkerBack);
			REQUIRE(store1.log[5].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[5].minute == 60);
			REQUIRE(store1.log[6].type == StoreEvent::WorkerBack);
			REQUIRE(store1.log[6].worker.resource == ResourceType::banana);
			REQUIRE(store1.log[6].minute == 60);
			REQUIRE(store1.log[7].type == StoreEvent::WorkerBack);
			REQUIRE(store1.log[7].worker.resource == ResourceType::schweppes);
			REQUIRE(store1.log[7].minute == 60);
			REQUIRE(store1.log[8].type == StoreEvent::ClientDepart);
			REQUIRE(store1.log[8].minute == 60);
			REQUIRE(store1.log[8].client.banana == 300);
			REQUIRE(store1.log[8].client.schweppes == 100);

			REQUIRE(store1.getBanana() == 0);
			REQUIRE(store1.getSchweppes() == 0);
			store1.advanceTo(61);//nothing should happen
			REQUIRE(store1.log.size() == 9);
		}
	}
	/*
	 1 1
	 75 20 50 3
	*/
	SECTION("Worker is sent for most wanted resource")
	{
		TestStore store2;
		store2.init(1, 0, 0);
		store2.addClients(Client{ 75, 20, 50, 3 });// 1 for schweppes
		REQUIRE(store2.log.size() == 0);
		SECTION("in the meantime")
		{
			store2.advanceTo(60);
			REQUIRE(store2.log.size() == 0);
		}
		SECTION("client is welcomed")
		{
			store2.advanceTo(75);
			REQUIRE(store2.log.size() == 1);
			REQUIRE(store2.log[0].type == StoreEvent::WorkerSend);
			REQUIRE(store2.log[0].worker.resource == ResourceType::schweppes);
			REQUIRE(store2.log[0].minute == 75);
		}
		SECTION("client departs with nothing at minute 78")
		{
			store2.advanceTo(78);
			REQUIRE(store2.log.size() == 2);
			REQUIRE(store2.log[1].type == StoreEvent::ClientDepart);
			REQUIRE(store2.log[1].client.banana == 0);
			REQUIRE(store2.log[1].client.schweppes == 0);
			REQUIRE(store2.getBanana() == 0);
			REQUIRE(store2.getSchweppes() == 0);
			REQUIRE(store2.log[1].minute == 78);
		}
		SECTION("Resources must arrive, regardless of there being no more clients arriving or waiting")
		{
			store2.advanceTo(135);
			REQUIRE(store2.log.size() == 3);
			REQUIRE(store2.log[2].type == StoreEvent::WorkerBack);
			REQUIRE(store2.log[2].worker.resource == ResourceType::schweppes);
			REQUIRE(store2.log[2].minute == 135);
			REQUIRE(store2.getBanana() == 0);
			REQUIRE(store2.getSchweppes() == 100);
		}
	}

/*
1  3
0 1 0 0
30 101 0 31
59 2 0 2
*/
	SECTION("Rearrangement happening")
	{
		TestStore store3;
		store3.init(1, 0, 0);
		store3.addClients({
			Client{0, 1, 0, 0},
			Client{30, 101, 0, 31},
			Client{59, 2, 0, 2},
			});
		REQUIRE(store3.log.size() == 0);
		SECTION("first client is welcomed")
		{
			store3.advanceTo(0);
			REQUIRE(store3.log.size() == 2);
			REQUIRE(store3.log[0].type == StoreEvent::WorkerSend);
			REQUIRE(store3.log[0].worker.resource == ResourceType::banana);
			REQUIRE(store3.log[0].minute == 0);
			//first client should go now because their wait time is 0
			REQUIRE(store3.log[1].type == StoreEvent::ClientDepart);
			REQUIRE(store3.log[1].client.banana == 0);
			REQUIRE(store3.log[1].client.schweppes == 0);
			REQUIRE(store3.log[1].minute == 0);
		}
		SECTION("second client is welcomed")
		{
			store3.advanceTo(30);
			REQUIRE(store3.log.size() == 2);// even though there is a need, 
											// no worker sould be sent since there aren't any available
		}
		SECTION("third client is welcomed")
		{
			store3.advanceTo(59);
			REQUIRE(store3.log.size() == 2);// even though there is a need, 
											// no worker sould be sent since there aren't any available
		}
		SECTION("Resources arrive and client with id 2 departs")
		{
			store3.advanceTo(60);
			REQUIRE(store3.log.size() == 4);
			REQUIRE(store3.log[2].type == StoreEvent::WorkerBack);
			REQUIRE(store3.log[2].worker.resource == ResourceType::banana);
			REQUIRE(store3.log[2].minute == 60);

			REQUIRE(store3.log[3].type == StoreEvent::ClientDepart);
			REQUIRE(store3.log[3].client.banana == 2);
			REQUIRE(store3.log[3].client.schweppes == 0);
			REQUIRE(store3.log[3].minute == 60);
			REQUIRE(store3.log[3].client.index == 2);// the first person who can be serviced is client 2 
												
			REQUIRE(store3.getBanana() == 98);
			REQUIRE(store3.getSchweppes() == 0);
		}
		SECTION("client with id 1 departs with what's left of banana")
		{
			store3.advanceTo(61);
			REQUIRE(store3.log.size() == 5);

			REQUIRE(store3.log[4].type == StoreEvent::ClientDepart);
			REQUIRE(store3.log[4].client.banana == 98);
			REQUIRE(store3.log[4].client.schweppes == 0);
			REQUIRE(store3.log[4].minute == 61);
			REQUIRE(store3.log[4].client.index == 1);// first the person who can be serviced/must go is serviced
													 // i.e. this happens by order of index
			REQUIRE(store3.getBanana() == 0);
			REQUIRE(store3.getSchweppes() == 0);
		}
	}

}

#endif
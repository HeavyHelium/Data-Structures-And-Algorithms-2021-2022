#pragma once
#pragma once
#ifndef MYTESTS_HPP
#include "catch.hpp"

TEST_CASE("Examples") {

	SECTION("one")
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
}


/*
5 4
0 10 0 10
45 35 0 30
46 30 20 100
200 10 10 1

Изход:
W 0 banana
0 10 0 0
W 46 schweppes
D 60 banana
1 60 35 0
D 106 schweppes
2 106 30 20
3 200 10 10


Изход:
W 0 banana
W 0 schweppes
D 60 banana
D 60 schweppes
0 60 50 50
1 60 50 50
W 60 banana
W 60 schweppes
2 60 0 0
3 110 0 0
D 120 banana
D 120 schweppes

*/

/*
 2 3
 0 15 20 65
 0 35 20 5
 10 5 0 5

Изход:
W 0 banana
W 0 schweppes
1 5 0 0
2 15 0 0
D 60 banana
D 60 schweppes
0 60 15 20


3 4
0  50 0 60
10 50 0 60
20 20 0 60
30 10 0 60

Изход:
W 0 banana
D 60 banana
0 60 50 0
1 60 50 0
2 80 0 0
3 90 0 0

2  2
0 20 0 61
1  90 0 59

Изход:
W 0 banana
D 60 banana
0 60 20 0
1 60 80 0

*/


#define MYTESTS_HPP
#endif
#pragma once
#ifndef INPUT_HPP
#define INPUT_HPP
#include <cstddef>
#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <cassert>

/// TODO make it a relative path
#include "..\..\..\..\hw1\hw1\interface.h"

/// @brief validates input
class InputHandler
{
	Client* clients = nullptr;
	int workers_cnt = 0;
	int clients_cnt = 0;
public:
	InputHandler() = default;
	InputHandler(const InputHandler& other) = delete;
	InputHandler& operator=(const InputHandler& other) = delete;
	~InputHandler() { delete[] clients; }

	void getInput()
	{
		workers_cnt = readANonNegativeInteger();
		clients_cnt = readANonNegativeInteger();
		clients = new Client[clients_cnt];
		std::string line;
		std::cin.ignore();
		for (std::size_t i = 0; i < clients_cnt; ++i)
		{
			std::getline(std::cin, line);
			if (isLineEmpty(line)) { --i; continue; }
			std::vector<std::string> parts = parseLine(line);
			inspect_parts(parts);
		}
		assert(cid == clients_cnt && "Must have reached end");
	}

	const Client* getClients() const { return clients; }
	std::size_t clientsCnt() const { return clients_cnt; }
	std::size_t workersCnt() const { return workers_cnt; }

	void print() const
	{
		std::cout << "W: " << workers_cnt << '\n';
		std::cout << "C: " << clients_cnt << '\n';

		for (std::size_t i = 0; i < clients_cnt; ++i)
			printClient(clients[i]);
	}
private:
	std::size_t cid = 0;
	void inspect_parts(const std::vector<std::string>& parts)
	{
		if (parts.size() != 4)
			throw std::logic_error("input format for a single client is: <arrival time> <banana> <schweppes> <wait time>");
		int arrival_time = std::stoi(parts[0]);
		int banana = std::stoi(parts[1]);
		int schweppes = std::stoi(parts[2]);
		int wait_time = std::stoi(parts[3]);
		if (arrival_time < 0 || banana < 0 || schweppes < 0 || wait_time < 0)
			throw std::invalid_argument("all values must be non-negative");
		clients[cid++] = Client{ arrival_time, banana, schweppes, wait_time };
	}
	static std::vector<std::string> parseLine(const std::string& line)
	{
		std::vector<std::string> parts;
		const char* str = line.c_str();
		std::string part;
		while (*str)
		{
			part.clear();
			while (isWhiteSpace(*str)) ++str;
			while (*str && !isWhiteSpace(*str)) part.push_back(*str++);
			if (!part.empty()) parts.push_back(part);
		}
		return parts;
	}
	static bool isWhiteSpace(char ch) { return ch == ' ' || ch == '\n'; }
	static int readAnInteger()
	{
		int value;
		std::cin >> value;
		if (!std::cin) throw std::logic_error("Bad input");
		return value;
	}
	static int readANonNegativeInteger()
	{
		int value = readAnInteger();
		if (value < 0) throw std::logic_error("Integer must be non-negative");
		return value;
	}
	static void printClient(const Client& c)
	{
		std::cout << c.arriveMinute << " " << c.banana << " "
			<< c.schweppes << " " << c.maxWaitTime << std::endl;
	}
	static bool isLineEmpty(const std::string& line)
	{
		const char* str = line.c_str();
		while (*str)
			if (!isWhiteSpace(*str++)) return false;
		return true;
	}
};

#endif // !INPUT_HPP

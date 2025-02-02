#pragma once

#include <cstddef>
#include <unordered_map>
#include <memory>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <fstream>
#include "waiting_queue.h"

namespace async {

using namespace std::chrono_literals;
using handle_t = unsigned long long;
std::string open_par = "{";
std::string close_par = "}";

// Потоки для логирования в консоль и в файлы
std::unique_ptr<std::thread> log = nullptr;
std::unique_ptr<std::thread> file1 = nullptr;
std::unique_ptr<std::thread> file2 = nullptr;

std::mutex conditionMutex_log;
std::condition_variable condition_log;
std::mutex conditionMutex_file;
std::condition_variable condition_file;


std::atomic<bool> active_connection(false);
std::atomic<bool> even(false);

unsigned long long context_count;

// номер контекста: <<кол-во скобок, размер буффера>, буффер комманд>
std::unordered_map<unsigned long long, std::pair<std::pair<std::size_t, std::size_t>, std::vector<std::string>>> context_map;
std::unordered_map<uint64_t, time_t> context_timestamp;
WaitingQueue<std::vector<std::string>> log_queue;
WaitingQueue<std::pair<uint64_t, std::vector<std::string>>> file_queue;

std::ofstream out1;
std::ofstream out2;

uint64_t timeSinceEpochMillisec();
handle_t connect(std::size_t bulk);
void receive(handle_t handle, const char *data, std::size_t size);
void disconnect(handle_t handle);

}

#include "async.h"

namespace async {

uint64_t timeSinceEpochMillisec() {
  using namespace std::chrono;
  std::this_thread::sleep_for(1ms);  // у меня слишком быстрый компьютер и поэтому несколько вызовов функции времени выдают одну и ту же метку
  return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// Поток логирования в консоль
void log_thread()
{
    std::unique_lock<std::mutex> lck{conditionMutex_log};
    std::vector<std::string> cmd_seq;
    bool pop_res;

    while(true)
    {
        while(log_queue.empty()) 
        {
            if(!active_connection)
                break;
            condition_log.wait(lck);
        }
        if(log_queue.empty() && !active_connection)
            break;
        else if(log_queue.empty())
            continue;
        pop_res = log_queue.tryPop(cmd_seq);
        if(!pop_res || cmd_seq.empty())
            continue;
        
        std::cout << "bulk: ";
        for (std::size_t i = 0; i < cmd_seq.size(); i++) //  Переделать на кол-во итераций, равное bulk
        {
            std::cout << cmd_seq[i];
            if(i < cmd_seq.size() - 1)
                std::cout << ", ";
        }
        std::cout << std::endl;

    }
}

// Нечётный поток логирования
void file1_thread()
{
    std::unique_lock<std::mutex> lck{conditionMutex_file};
    std::pair<uint64_t, std::vector<std::string>> cmd_seq;
    bool pop_res;
    while(true)
    {
        while(file_queue.empty()) 
        {
            if(!active_connection)
                break;
            condition_file.wait(lck);
        }
        if(file_queue.empty() && !active_connection)
            break;
        else if(even || file_queue.empty())
        {
            even = !even;
            continue;
        }
        pop_res = file_queue.tryPop(cmd_seq);
        if(!pop_res || cmd_seq.second.empty())
            continue;
        out1.open(std::to_string(cmd_seq.first) + std::string("_1") + std::string(".log"));
        out1 << std::to_string(cmd_seq.first) << ": ";
        for (unsigned int i = 0; i < cmd_seq.second.size(); i++)
        {
            out1 << cmd_seq.second[i];
            if(i < cmd_seq.second.size() - 1)
                out1 << ", ";
        }
        out1 << std::endl;
        out1.close();
        even = !even;
    }
    even = true;
}

// Чётный поток логирования
void file2_thread()
{
    std::unique_lock<std::mutex> lck{conditionMutex_file};
    std::pair<uint64_t, std::vector<std::string>> cmd_seq;
    bool pop_res;
    while(true)
    {
        while(file_queue.empty()) 
        {
            if(!active_connection)
                break;
            condition_file.wait(lck);
        }
        if(file_queue.empty() && !active_connection)
            break;
        else if(!even || file_queue.empty())
        {
            even = !even;
            continue;
        }
        pop_res = file_queue.tryPop(cmd_seq);
        if(!pop_res || cmd_seq.second.empty())
            continue;
        out2.open(std::to_string(cmd_seq.first) + std::string("_2") + std::string(".log"));
        out2 << std::to_string(cmd_seq.first) << ": ";
        for (unsigned int i = 0; i < cmd_seq.second.size(); i++)
        {
            out2 << cmd_seq.second[i];
            if(i < cmd_seq.second.size() - 1)
                out2 << ", ";
        }
        out2 << std::endl;
        out2.close();
        even = !even;
    }
    even = false;
}

handle_t connect(std::size_t bulk) {
    if(!active_connection)
    {
        active_connection = true;
        log = std::make_unique<std::thread>(log_thread);
        file1 = std::make_unique<std::thread>(file1_thread);
        file2 = std::make_unique<std::thread>(file2_thread);
    }
    context_count++;
    context_map[context_count] = std::make_pair<>(std::make_pair<>(0, bulk), std::vector<std::string>());
    return context_count;
}

void receive(handle_t handle, const char *data, std::size_t size) {
    std::string line;
    for (size_t i = 0; i < size; i++) // Цикл по буфферу команд
    {
        line.clear();
        while(i < size && data[i] != '\n') // Отделяем одну команду
        {
            line += data[i];
            i++;
        }

        if(line.size() == 0)
            continue;
        i--;
        
        if(line == open_par) // Обработка открывающей скобки
        {
            if(context_map[handle].first.first == 0)  // Вывод накопленной последовательности команд если это первая скобка
            {
                file_queue.push(std::make_pair(context_timestamp[handle], std::vector<std::string>(context_map[handle].second)));
                log_queue.push(std::move(context_map[handle].second));
                context_map[handle].second = std::vector<std::string>();
                condition_log.notify_all();
                condition_file.notify_all();
            }
            context_map[handle].first.first++;
        }
        else if(line == close_par) // Обработка закрывающей скобки
        {
            if(context_map[handle].first.first == 1) // Вывод только в случае если скобка одна
            {
                file_queue.push(std::make_pair(context_timestamp[handle], std::vector<std::string>(context_map[handle].second)));
                log_queue.push(std::move(context_map[handle].second));
                context_map[handle].second = std::vector<std::string>();
                condition_log.notify_all();
                condition_file.notify_all();
            }
            context_map[handle].first.first--;
            continue;
        }
        else // Передача команды в буффер
        {
            if(context_map[handle].second.size() == 0)
            {
                context_timestamp[handle] = timeSinceEpochMillisec();
            }
            context_map[handle].second.push_back(line);
        }
            
        if(context_map[handle].first.first == 0 && context_map[handle].second.size() == context_map[handle].first.second)  // Вывод накопленной очереди команд
        {
            file_queue.push(std::make_pair(context_timestamp[handle], std::vector<std::string>(context_map[handle].second)));
            log_queue.push(std::move(context_map[handle].second));
            context_map[handle].second = std::vector<std::string>();
            // std::cout << "Notify all 3\n";
            condition_log.notify_all();
            condition_file.notify_all();
        }
    }
    
}

void disconnect(handle_t handle) {
    file_queue.push(std::make_pair(context_timestamp[handle], std::vector<std::string>(context_map[handle].second)));
    log_queue.push(std::move(context_map[handle].second));
    condition_log.notify_all();
    condition_file.notify_all();
    context_map.erase(handle);
    context_timestamp.erase(handle);
    if(context_map.empty())
    {
        active_connection = false;
        condition_log.notify_all();
        condition_file.notify_all();
        log->join();
        file1->join();
        file2->join();
    }
}
}

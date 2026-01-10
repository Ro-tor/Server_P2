#include<iostream>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include <sstream>
#include <algorithm>

const int PORT = 8080;
const int BUFFER_SIZE = 1025;

// Функция 
int find_max_remainder(int number, int limit) {
    int max_remainder = 0;
    
    for (int divisor = 1; divisor <= limit; divisor++) {
        int remainder = number % divisor;
        if (remainder > max_remainder) {
            max_remainder = remainder;
        }
    }
    
    return max_remainder;
}

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocket < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }
    
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);
    
    if (bind(serverSocket, 
             (sockaddr*)&serverAddr, 
             sizeof(serverAddr)) < 0) {
        close(serverSocket);
        std::cerr << "Ошибка привязки сокета" << std::endl;
        return 1;
    }
    
    if (listen(serverSocket, 3) < 0) {
        close(serverSocket);
        std::cerr << "Ошибка прослушивания" << std::endl;
        return 2;
    }
    
    std::cout << "Сервер запущен на порту " << PORT << std::endl;
    std::cout << "Ожидание подключения клиента..." << std::endl;

    // Принятие подключения
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket, 
                            (sockaddr*)&clientAddr,
                            &clientAddrLen);
    if (clientSocket < 0) {
        close(serverSocket);
        std::cerr << "Ошибка принятия подключения" << std::endl;
        return 3;
    }
    
    // Получение IP клиента
    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, 
            &clientAddr.sin_addr, 
            clientIP, 
            INET_ADDRSTRLEN);
    std::cout << "Клиент подключен: " << clientIP << std::endl;

    // Основной цикл обработки сообщений
    char BUFFER[BUFFER_SIZE];
    while (true) {
        memset(BUFFER, 0, BUFFER_SIZE);
        
        // Получение данных от клиента
        int byteRecieved = recv(clientSocket, BUFFER, BUFFER_SIZE - 1, 0);
        if (byteRecieved <= 0) {
            std::cout << "Клиент отключился" << std::endl;
            break;
        }
        
        std::string message(BUFFER);
        std::cout << "Получено от клиента: " << message << std::endl;
        
        // Проверка на команду выхода
        if (message == "exit") {
            std::cout << "Завершение работы по команде клиента" << std::endl;
            break;
        }
        
        // Обработка запроса
        std::string response;
        
        // Формат: число,предел
        size_t comma_pos = message.find(',');
        if (comma_pos != std::string::npos) {
            try {
                int number = std::stoi(message.substr(0, comma_pos));
                int limit = std::stoi(message.substr(comma_pos + 1));
                
                if (limit <= 0 || number <= 0) {
                    response = "Ошибка: числа должны быть положительными";
                } else if (limit > 1000000) {
                    response = "Ошибка: предел слишком большой (макс. 1000000)";
                } else {
                    int result = find_max_remainder(number, limit);
                    response = "Максимальный остаток от деления " + 
                               std::to_string(number) + 
                               " на числа 1..." + 
                               std::to_string(limit) + 
                               " = " + 
                               std::to_string(result);
                }
            } catch (const std::exception& e) {
                response = "Ошибка: неверный формат данных. Используйте: число,предел";
            }
        } else {
            response = "Ошибка: неверный формат данных. Используйте: число,предел";
        }
        
        // Отправка ответа клиенту
        send(clientSocket, 
            response.c_str(), 
            response.length(), 
            0);
        std::cout << "Отправлено клиенту: " << response << std::endl;
    }
    
    close(clientSocket);
    close(serverSocket);
    std::cout << "Сервер завершил работу" << std::endl;
    
    return 0;
}
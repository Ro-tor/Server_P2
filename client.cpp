#include<iostream>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

const int SERVERPORT = 8080;
const int BUFFER_SIZE = 1025;
const char* SERVERIP = "127.0.0.1";

int main() {
    int clientSocket = socket(AF_INET, 
                              SOCK_STREAM, 
                              0);
    if(clientSocket < 0) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        return 1;
    }
    
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVERPORT);
    
    if (inet_pton(AF_INET, SERVERIP, &serverAddr.sin_addr) <= 0) {
        std::cerr << "Неверный адрес сервера" << std::endl;
        close(clientSocket);
        return 2;
    }

    std::cout << "Подключение к серверу " << SERVERIP << ":" << SERVERPORT << "..." << std::endl;
    if (connect(clientSocket,
                (sockaddr*)&serverAddr,
                sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка подключения к серверу. Убедитесь, что сервер запущен." << std::endl;
        close(clientSocket);
        return 3;
    }
    
    std::cout << "Успешное подключение к серверу!" << std::endl;
    std::cout << "Введите 'exit' для выхода" << std::endl;
    std::cout << "Формат запроса: число,предел (например: 2015,1000)" << std::endl;
    
    char buffer[BUFFER_SIZE];

    while (true) {
        std::cout << "Введите запрос: ";
        std::string message;
        std::getline(std::cin, message);
        
        if (message == "exit") {
            send(clientSocket, "exit", 4, 0);
            std::cout << "Завершение работы..." << std::endl;
            break;
        }
        
        // Отправка запроса серверу
        send(clientSocket,
             message.c_str(),
             message.length(),
             0);
        
        // Получение ответа от сервера
        memset(buffer, 0, BUFFER_SIZE);
        int byteRecieved = recv(clientSocket,
                                buffer,
                                BUFFER_SIZE - 1,
                                0);
        if (byteRecieved > 0) {
            std::cout << "Ответ сервера: " << buffer << std::endl;
        } else if (byteRecieved == 0) {
            std::cout << "Сервер закрыл соединение" << std::endl;
            break;
        } else {
            std::cerr << "Ошибка при получении ответа" << std::endl;
            break;
        }
    }
    
    close(clientSocket);
    std::cout << "Клиент завершил работу" << std::endl;
    
    return 0;
}
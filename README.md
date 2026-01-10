# Server_P2

Client-server application for solving the math problem: finding the maximum balance of dividing a given number by all numbers in the specified range.
(Project)

# Running the task (Codespace)

## 1. Start the server (in a separate terminal):

bash
    
    ./server

Output: 
        
        "Сервер запущен на порту 8080..."
        
        "Ожидание подключения клиента..."

Output after connection:

        Клиент подключен: 127.0.0.1

Output after the solution:

        Получено от клиента: (данные, кот. были введены пользователем)

        Отправлено клиенту: Максимальный остаток от деления ... на числа 1... = ...

## 2. Start the client (in a other terminal):

bash
    
    ./client

Output: 
        
        Подключение к серверу 127.0.0.1:8080...
        
        Успешное подключение к серверу!
        
        "Введите запрос: "
        
        Enter the details: (Example: 2015,1000)
        
        Answer: "Максимальный остаток от деления 2015 на числа 1...1000 = ..."

## 3. Exit request:

bash

    exit

Output: 
        
        Завершение работы...
        
        Клиент завершил работу

## Implementation features:

* Error handling: Checking the correctness of input data

* Request format: number,limit (by comma)

* Protection: Check for positive numbers and limit limit

* Logon: The server displays information about connections and requests

* Flexibility: Can send multiple requests without reconnecting
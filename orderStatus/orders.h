#ifndef ORDERS_H
#define ORDERS_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <ctime>
#include <queue>
#include <list>

using namespace std;

enum OrderStatus {
    PREPARING,
    DISPATCHED,
    ON_THE_WAY,
    DELIVERED
};

string getStatusString(OrderStatus status);

class Order {
public:
    int orderId;
    string customerName;
    list<pair<time_t, OrderStatus>> statusHistory;

    Order(int id, const string& name);
    void updateStatus(OrderStatus newStatus);
    void displayStatusHistory() const;
    OrderStatus getCurrentStatus() const;
    string formatTime(time_t time) const; // Add this method declaration
};

class OrderManager {
private:
    unordered_map<int, Order> orders;
    queue<int> updateQueue;

public:
    void createOrder(int orderId, const string& customerName);
    void updateOrderStatus(int orderId, OrderStatus newStatus);
    void processUpdates();
    void notifyUser(int orderId);
    void displayOrderStatusHistory(int orderId) const;
    OrderStatus getOrderStatus(int orderId) const;
};

#endif // ORDERS_H
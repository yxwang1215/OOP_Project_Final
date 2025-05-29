//
// Created by liu on 5/11/25.
//

#include "orderList.h"
#include <string>
#include <fstream>
#include <ranges>
#include <sstream>
#include "log.h"
#include <thread>
#include <iostream>

OrderList::OrderList() {
    std::ifstream orderFile("order.txt");
    if (!orderFile.is_open()) {
        logger << true << LogString::FileNotFound;
        return;
    }
    std::string orderId;
    long long time;
    std::string order_status_str;
    std::vector<std::pair<std::string, int>> items;
    double total;
    std::string address, orderUser;
    while (orderFile >> orderId >> orderUser >> time >> order_status_str) {
        items.clear();
        Order::orderStatus order_status;
        if (order_status_str == "PENDING") {
            order_status = Order::PENDING;
        }
        else if (order_status_str == "SHIPPED") {
            order_status = Order::SHIPPED;
        }
        else if (order_status_str == "DELIVERED") {
            order_status = Order::DELIVERED;
        }
        else if (order_status_str == "CANCELLED") {
            order_status = Order::CANCELLED;
        }
        orderFile.ignore();
        std::string items_str;
        std::getline(orderFile, items_str);
        std::stringstream items_stream(items_str);
        std::string item;
        int quantity;
        while (items_stream >> item >> quantity) {
            items.emplace_back(item, quantity);
        }
        orderFile >> total;
        orderFile.ignore();
        std::getline(orderFile, address);
        Order order(orderId, orderUser, time, order_status, items, total, address);
        orderMap.insert({orderId, order});
    }
    orderFile.close();
}

void OrderList::writeToFile() {
    std::ofstream orderFile("order.txt");
    if (!orderFile.is_open()) {
        logger << true << LogString::FileNotFound;
        return;
    }
    for (const auto& [orderId, order] : orderMap) {
        orderFile << orderId << " " << order.getOrderUser() << " " << std::chrono::duration_cast<std::chrono::seconds>(
            order.getTime().time_since_epoch()).count() << " ";
        switch (order.getOrderStatus()) {
        case Order::PENDING:
            orderFile << "PENDING";
            break;
        case Order::SHIPPED:
            orderFile << "SHIPPED";
            break;
        case Order::DELIVERED:
            orderFile << "DELIVERED";
            break;
        case Order::CANCELLED:
            orderFile << "CANCELLED";
            break;
        default: ;
        }
        orderFile << std::endl;
        for (const auto& [item, quantity] : order.getItems()) {
            orderFile << item << " " << quantity << " ";
        }
        orderFile << std::endl;
        orderFile << order.getTotal() << "\n" << order.getAddress() << "\n";
    }
    orderFile.close();
}

bool OrderList::addOrder(const std::string& orderUser, const std::vector<std::pair<std::string, int>>& items,
                         const std::string& address, const Coupon& coupon) {
    const Order order(orderUser, items, address, coupon);
    std::string orderId = order.getOrderId();
    logger << true << "Your order id is " << orderId << "\n";
    logger << "Your address is " << address << "\n";
    logger << "Your total is " << order.getTotal() << "\n";
    logger << "Submit(y/n): ";
    std::string submit;
    std::cin >> submit;
    if (submit != "y" && submit != "Y") {
        return false;
    }
    orderMap.insert({orderId, order});
    std::thread changeOrderStatusThread(&OrderList::changeOrderStatus, this, orderId, Order::SHIPPED, 10);
    changeOrderStatusThread.detach();
    writeToFile();
    return true;
}

bool OrderList::deleteOrder(const std::string& orderId) {
    orderMap.erase(orderId);
    writeToFile();
    return true;
}

void OrderList::printOrder(const std::string& orderId) const {
    if (orderMap.contains(orderId)) {
        logger << orderMap.at(orderId).toString();
    }
    else {
        logger << true << LogString::OrderNotFound;
    }
}

bool OrderList::hasOrder(const std::string& orderId) const {
    return orderMap.contains(orderId);
}

bool OrderList::changeOrderStatus(const std::string& orderId, const Order::orderStatus status, const int sleep) {
    std::this_thread::sleep_for(std::chrono::seconds(sleep));
    if (orderMap.contains(orderId)) {
        orderMap.at(orderId).setOrderStatus(status);
        writeToFile();
        return true;
    }
    return false;
}

Order::orderStatus OrderList::getOrderStatus(const std::string& orderId) const {
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getOrderStatus();
    }
    return Order::PENDING;
}

bool OrderList::setOrderAddress(const std::string& orderId, const std::string& address) {
    if (orderMap.contains(orderId)) {
        orderMap.at(orderId).setOrderAddress(address);
        writeToFile();
        return true;
    }
    return false;
}

std::string OrderList::getOrderUser(const std::string& orderId) const {
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getOrderUser();
    }
    return "";
}

std::vector<std::string> OrderList::getOrderList(const std::string& orderUser) {
    std::vector<std::string> orderList;
    for (const auto& [orderId, order] : orderMap) {
        if (order.getOrderUser() == orderUser) {
            orderList.push_back(orderId);
        }
    }
    return orderList;
}

void OrderList::printAllOrders() const {
    for (const auto& order : orderMap | std::views::values) {
        logger << order.toString();
        logger << LogString::Hr;
    }
}

std::vector<std::pair<std::string, int>> OrderList::getOrderItems(const std::string& orderId) const {
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getItems();
    }
    return {};
}

//
// Created by liu on 5/11/25.
//

#ifndef ORDERLIST_H
#define ORDERLIST_H

#include <map>
#include <vector>
#include "order.h"
#include "coupon.h"
class OrderList {
public:
    OrderList();
    void writeToFile();
    bool addOrder(const std::string&, const std::vector<std::pair<std::string, int>>& items,
                  const std::string& address, const Coupon&);
    bool deleteOrder(const std::string& orderId);
    void printOrder(const std::string& orderId) const;
    [[nodiscard]] bool hasOrder(const std::string& orderId) const;
    bool changeOrderStatus(const std::string& orderId, Order::orderStatus status, int);
    [[nodiscard]] Order::orderStatus getOrderStatus(const std::string& orderId) const;
    bool setOrderAddress(const std::string&, const std::string&);
    [[nodiscard]] std::string getOrderUser(const std::string&) const;
    std::vector<std::string> getOrderList(const std::string&);
    void printAllOrders() const;
    [[nodiscard]] std::vector<std::pair<std::string, int>> getOrderItems(const std::string&) const;
private:
    std::map<std::string, Order> orderMap;
};

#endif //ORDERLIST_H

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <chrono>
#include <vector>
#include "coupon.h"
#include <tuple>

using sys_seconds_tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;


class Order {
public:
    enum orderStatus {
        PENDING,
        SHIPPED,
        DELIVERED,
        CANCELLED
    };

    Order(const std::string&, const std::vector<std::pair<std::string, int>>&, const std::string&, const Coupon&);
    Order(const std::string&, const std::string&, long long, orderStatus,
          const std::vector<std::pair<std::string, int>>&, double, const std::string&);
    // Getters
    [[nodiscard]] std::string getOrderId() const;
    [[nodiscard]] std::string getOrderUser() const;
    [[nodiscard]] sys_seconds_tp getTime() const;
    [[nodiscard]] orderStatus getOrderStatus() const;
    [[nodiscard]] std::vector<std::pair<std::string, int>> getItems() const;
    [[nodiscard]] double getTotal() const;
    [[nodiscard]] std::string getAddress() const;

    // Setters
    void setOrderStatus(orderStatus status);
    void setOrderAddress(const std::string&);

    // To string
    [[nodiscard]] std::string toString() const;
    static std::string format_timepoint_utc_seconds(const sys_seconds_tp&);
    static std::string format_timepoint_local_seconds(const sys_seconds_tp& tp_s);

private:
    std::string orderId;
    std::string orderUser;
    sys_seconds_tp time;
    orderStatus order_status;
    std::vector<std::pair<std::string, int>> items;
    double total;
    std::string address;
};

#endif

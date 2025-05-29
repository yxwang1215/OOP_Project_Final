#include "order.h"
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "productList.h" // Include productList.h
#include <format>
// Helper function to generate a random string
std::string generate_random_string(const size_t length) {
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, characters.length() - 1);

    std::string random_string;
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }
    return random_string;
}

Order::Order(const std::string& orderUser, const std::vector<std::pair<std::string, int>>& items,
             const std::string& address, const Coupon& coupon) {
    this->orderId = generate_random_string(5); // Generate 5-character random string
    this->address = address;
    this->items = items;
    this->order_status = PENDING;
    this->time = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    this->total = 0;
    this->orderUser = orderUser;
    for (const auto& [fst, snd] : this->items) {
        ProductList productList;
        const double price = productList.getPrice(fst);
        this->total += price * snd * std::get<0>(coupon.getCouponDiscount(fst, productList.getCategory(fst)));
    }
    if (this->total >= coupon.getMinimum()) {
        this->total -= coupon.getDiscount();
    }
}

Order::Order(const std::string& orderId, const std::string& orderUser, const long long time,
             const orderStatus order_status, const std::vector<std::pair<std::string, int>>& items, const double total,
             const std::string& address) {
    this->orderId = orderId;
    this->time = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(std::chrono::seconds(time));
    this->order_status = order_status;
    this->items = items;
    this->total = total;
    this->address = address;
    this->orderUser = orderUser;
}

// Getters
std::string Order::getOrderId() const {
    return orderId;
}

sys_seconds_tp Order::getTime() const {
    return time;
}

Order::orderStatus Order::getOrderStatus() const {
    return order_status;
}

std::vector<std::pair<std::string, int>> Order::getItems() const {
    return items;
}

double Order::getTotal() const {
    return total;
}

std::string Order::getAddress() const {
    return address;
}

// Setters
void Order::setOrderStatus(orderStatus status) {
    this->order_status = status;
}

std::string Order::format_timepoint_utc_seconds(const sys_seconds_tp& tp_s) {
    return std::format("{:%Y-%m-%d %H:%M:%S} UTC", tp_s);
}

std::string Order::format_timepoint_local_seconds(const sys_seconds_tp& tp_s) {
    auto zt = std::chrono::zoned_time(std::chrono::current_zone(), tp_s);
    return std::format("{:%Y-%m-%d %H:%M:%S %Z}", zt);
}

std::string Order::toString() const {
    std::string result = "Order ID: " + this->orderId + "\t\t";

    result += "Time: " + format_timepoint_local_seconds(this->time) + "\t";
    result += "Order Status: ";
    switch (this->order_status) {
    case PENDING:
        result += "Pending";
        break;
    case SHIPPED:
        result += "Shipped";
        break;
    case DELIVERED:
        result += "Delivered";
        break;
    case CANCELLED:
        result += "Cancelled";
        break;
    default: ;
    }
    result += "\t";
    result += "User: " + this->orderUser;
    result += "\nItems:\n";
    for (const auto& [fst, snd] : this->items) {
        result += "\t- Name: " + fst + ", Quantity: " + std::to_string(snd) + "\n";
    }
    result += "Total: $" + std::to_string(this->total) + "\t";
    result += "Address: " + this->address + "\n";
    return result;
}

std::string Order::getOrderUser() const {
    return orderUser;
}

void Order::setOrderAddress(const std::string& address) {
    this->address = address;
}

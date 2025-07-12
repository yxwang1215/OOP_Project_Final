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

// OrderList 类构造函数
// 从 order.txt 文件加载订单信息
OrderList::OrderList() {
    // 打开 order.txt 文件
    std::ifstream orderFile("order.txt");
    // 检查文件是否成功打开
    if (!orderFile.is_open()) {
        logger << true << LogString::FileNotFound; // 打印文件未找到错误日志
        return; // 如果文件未打开，则返回
    }
    std::string orderId; // 订单ID
    long long time; // 订单时间（秒）
    std::string order_status_str; // 订单状态字符串
    std::vector<std::pair<std::string, int>> items; // 订单商品列表
    double total; // 订单总金额
    std::string address, orderUser; // 收货地址，下单用户
    // 循环读取每个订单的信息
    while (orderFile >> orderId >> orderUser >> time >> order_status_str) {
        items.clear(); // 清空商品列表，为下一个订单做准备
        Order::orderStatus order_status; // 订单状态枚举
        // 根据读取的字符串设置订单状态枚举值
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
        orderFile.ignore(); // 忽略当前行的剩余内容（包括换行符）
        std::string items_str; // 商品列表字符串
        std::getline(orderFile, items_str); // 读取商品列表字符串
        std::stringstream items_stream(items_str); // 创建字符串流以便解析商品信息
        std::string item; // 商品名称
        int quantity; // 商品数量
        // 循环从商品列表字符串流中读取商品名称和数量
        while (items_stream >> item >> quantity) {
            items.emplace_back(item, quantity); // 将商品名称和数量作为一个pair添加到商品列表中
        }
        orderFile >> total; // 读取订单总金额
        orderFile.ignore(); // 忽略当前行的剩余内容（包括换行符）
        std::getline(orderFile, address); // 读取收货地址
        // 创建 Order 对象并添加到 orderMap 中
        Order order(orderId, orderUser, time, order_status, items, total, address);
        orderMap.insert({orderId, order});
    }
    orderFile.close(); // 关闭文件
}

// 将订单信息写入文件
void OrderList::writeToFile() {
    // 打开 order.txt 文件进行写入
    std::ofstream orderFile("order.txt");
    // 检查文件是否成功打开
    if (!orderFile.is_open()) {
        logger << true << LogString::FileNotFound; // 打印文件未找到错误日志
        return; // 如果文件未打开，则返回
    }
    // 遍历 orderMap 中的所有订单
    for (const auto& [orderId, order] : orderMap) {
        // 写入订单ID、下单用户和订单时间（秒）
        orderFile << orderId << " " << order.getOrderUser() << " " << std::chrono::duration_cast<std::chrono::seconds>(
            order.getTime().time_since_epoch()).count() << " ";
        // 根据订单状态枚举值写入对应的状态字符串
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
        orderFile << std::endl; // 换行
        // 遍历订单商品列表，写入每个商品的名称和数量
        for (const auto& [item, quantity] : order.getItems()) {
            orderFile << item << " " << quantity << " ";
        }
        orderFile << std::endl; // 换行
        // 写入订单总金额和收货地址
        orderFile << order.getTotal() << "\n" << order.getAddress() << "\n";
    }
    orderFile.close(); // 关闭文件
}

// 添加新订单
// orderUser: 下单用户
// items: 订单包含的商品列表
// address: 收货地址
// coupon: 优惠券对象引用
// 如果订单成功提交返回 true，否则返回 false
bool OrderList::addOrder(const std::string& orderUser, const std::vector<std::pair<std::string, int>>& items,
                         const std::string& address, Coupon& coupon) {
    // 创建新的 Order 对象
    const Order order(orderUser, items, address, coupon);
    std::string orderId = order.getOrderId(); // 获取订单ID
    // 打印订单信息给用户确认
    logger << true << "Your order id is " << orderId << "\n";
    logger << "Your address is " << address << "\n";
    logger << "Your total is " << order.getTotal() << "\n";
    logger << "Submit(y/n): "; // 询问用户是否提交订单
    std::string submit; // 用户输入
    std::cin >> submit; // 读取用户输入
    // 如果用户输入不是 'y' 或 'Y'，则取消订单
    if (submit != "y" && submit != "Y") {
        return false; // 返回 false 表示订单未提交
    }
    orderMap.insert({orderId, order}); // 将订单添加到 orderMap 中
    // 创建一个新线程，在指定时间后自动将订单状态改为 SHIPPED
    std::thread changeOrderStatusThread(&OrderList::changeOrderStatus, this, orderId, Order::SHIPPED, 10);
    changeOrderStatusThread.detach(); // 分离线程，使其独立运行
    writeToFile(); // 将更新后的订单信息写入文件
    return true; // 返回 true 表示订单提交成功
}

// 删除订单
// orderId: 要删除的订单ID
// 如果删除成功返回 true，否则返回 false
bool OrderList::deleteOrder(const std::string& orderId) {
    orderMap.erase(orderId); // 从 orderMap 中删除指定订单
    writeToFile(); // 将更新后的订单信息写入文件
    return true; // 返回 true 表示删除成功
}

// 打印指定订单的信息
// orderId: 要打印的订单ID
void OrderList::printOrder(const std::string& orderId) const {
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        logger << orderMap.at(orderId).toString(); // 如果包含，则打印订单的字符串表示
    }
    else {
        logger << true << LogString::OrderNotFound; // 如果不包含，则打印订单未找到错误日志
    }
}

// 检查是否存在指定订单
// orderId: 要检查的订单ID
// 如果存在返回 true，否则返回 false
bool OrderList::hasOrder(const std::string& orderId) const {
    return orderMap.contains(orderId); // 检查 orderMap 中是否包含指定订单
}

// 修改订单状态
// orderId: 要修改状态的订单ID
// status: 新的订单状态
// sleep: 延迟修改的时间（秒）
// 如果修改成功返回 true，否则返回 false
bool OrderList::changeOrderStatus(const std::string& orderId, const Order::orderStatus status, const int sleep) {
    std::this_thread::sleep_for(std::chrono::seconds(sleep)); // 线程休眠指定时间
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        orderMap.at(orderId).setOrderStatus(status); // 设置订单状态
        writeToFile(); // 将更新后的订单信息写入文件
        return true; // 返回 true 表示修改成功
    }
    return false; // 返回 false 表示修改失败（订单不存在）
}

// 获取订单状态
// orderId: 要获取状态的订单ID
// 返回订单状态枚举值，如果订单不存在则返回 PENDING
Order::orderStatus OrderList::getOrderStatus(const std::string& orderId) const {
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getOrderStatus(); // 如果包含，则返回订单状态
    }
    return Order::PENDING; // 如果不包含，则返回 PENDING
}

// 设置订单收货地址
// orderId: 要设置地址的订单ID
// address: 新的收货地址
// 如果设置成功返回 true，否则返回 false
bool OrderList::setOrderAddress(const std::string& orderId, const std::string& address) {
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        orderMap.at(orderId).setOrderAddress(address); // 设置订单收货地址
        writeToFile(); // 将更新后的订单信息写入文件
        return true; // 返回 true 表示设置成功
    }
    return false; // 返回 false 表示设置失败（订单不存在）
}

// 获取订单的下单用户
// orderId: 要获取下单用户的订单ID
// 返回下单用户字符串，如果订单不存在则返回空字符串
std::string OrderList::getOrderUser(const std::string& orderId) const {
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getOrderUser(); // 如果包含，则返回下单用户
    }
    return ""; // 如果不包含，则返回空字符串
}

// 获取指定用户的所有订单ID列表
// orderUser: 用户名
// 返回包含该用户所有订单ID的向量
std::vector<std::string> OrderList::getOrderList(const std::string& orderUser) {
    std::vector<std::string> orderList; // 用于存储订单ID的向量
    // 遍历 orderMap 中的所有订单
    for (const auto& [orderId, order] : orderMap) {
        // 如果订单的下单用户与指定用户匹配
        if (order.getOrderUser() == orderUser) {
            orderList.push_back(orderId); // 将订单ID添加到列表中
        }
    }
    return orderList; // 返回订单ID列表
}

// 打印所有订单的信息
void OrderList::printAllOrders() const {
    // 遍历 orderMap 中的所有订单值
    for (const auto& order : orderMap | std::views::values) {
        logger << order.toString(); // 打印订单的字符串表示
        logger << LogString::Hr; // 打印分隔线
    }
}

// 获取指定订单的商品列表
// orderId: 要获取商品列表的订单ID
// 返回包含商品名称和数量对的向量，如果订单不存在则返回空向量
std::vector<std::pair<std::string, int>> OrderList::getOrderItems(const std::string& orderId) const {
    // 检查 orderMap 中是否包含指定订单
    if (orderMap.contains(orderId)) {
        return orderMap.at(orderId).getItems(); // 如果包含，则返回订单的商品列表
    }
    return {}; // 如果不包含，则返回空向量
}

// 获取所有订单的列表
// 返回包含所有订单对象的向量
std::vector<Order> OrderList::getAllOrders() const {
    std::vector<Order> orders; // 用于存储订单对象的向量
    // 遍历 orderMap 中的所有订单值
    for (const auto& order : orderMap | std::views::values) {
        orders.push_back(order); // 将订单对象添加到向量中
    }
    return orders; // 返回所有订单的向量
}

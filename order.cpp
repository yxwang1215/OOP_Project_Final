#include "order.h"
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include "productList.h" // Include productList.h
#include <format>
// Helper function to generate a random string
// 辅助函数：生成指定长度的随机字符串
// length: 随机字符串的长度
// 返回生成的随机字符串
std::string generate_random_string(const size_t length) {
    // 定义包含所有可能字符的字符串
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    // 创建随机设备
    std::random_device random_device;
    // 使用随机设备初始化 Mersenne Twister 引擎
    std::mt19937 generator(random_device());
    // 创建均匀整数分布，范围为字符索引
    std::uniform_int_distribution<> distribution(0, characters.length() - 1);

    std::string random_string; // 用于存储生成的随机字符串
    // 循环指定次数，生成随机字符并添加到字符串
    for (size_t i = 0; i < length; ++i) {
        random_string += characters[distribution(generator)];
    }
    // 返回生成的随机字符串
    return random_string;
}

// 订单类构造函数，用于创建新订单
// orderUser: 下单用户
// items: 订单包含的商品列表（商品名称和数量对的向量）
// address: 收货地址
// coupon: 优惠券对象引用
Order::Order(const std::string& orderUser, const std::vector<std::pair<std::string, int>>& items,
             const std::string& address, Coupon& coupon) {
    this->orderId = generate_random_string(5); // 生成 5 个字符的随机订单ID
    this->address = address; // 设置收货地址
    this->items = items; // 设置订单商品列表
    this->order_status = PENDING; // 设置订单状态为待处理
    this->time = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now()); // 设置订单时间为当前时间
    this->total = 0; // 初始化订单总金额为 0
    this->orderUser = orderUser; // 设置下单用户
    // 计算订单总金额，考虑商品价格和优惠券折扣
    for (const auto& [fst, snd] : this->items) {
        ProductList productList; // 创建 ProductList 对象以获取商品信息
        const double price = productList.getPrice(fst); // 获取商品价格
        // 计算商品总价并累加到订单总金额，应用商品或类别优惠券折扣
        this->total += price * snd * std::get<0>(coupon.getCouponDiscount(fst, productList.getCategory(fst)));
    }
    // 如果订单总金额达到满减优惠的最低消费金额，则应用满减折扣
    if (this->total >= coupon.getMinimum()) {
        this->total -= coupon.getDiscount();
    }
}

// 订单类构造函数，用于加载现有订单
// orderId: 订单ID
// orderUser: 下单用户
// time: 订单时间（秒）
// order_status: 订单状态
// items: 订单包含的商品列表
// total: 订单总金额
// address: 收货地址
Order::Order(const std::string& orderId, const std::string& orderUser, const long long time,
             const orderStatus order_status, const std::vector<std::pair<std::string, int>>& items, const double total,
             const std::string& address) {
    this->orderId = orderId; // 设置订单ID
    // 将时间（秒）转换为时间点
    this->time = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(std::chrono::seconds(time));
    this->order_status = order_status; // 设置订单状态
    this->items = items; // 设置订单商品列表
    this->total = total; // 设置订单总金额
    this->address = address; // 设置收货地址
    this->orderUser = orderUser; // 设置下单用户
}

// Getters
// 获取订单ID
// 返回订单ID字符串
std::string Order::getOrderId() const {
    return orderId;
}

// 获取订单时间
// 返回订单时间点
sys_seconds_tp Order::getTime() const {
    return time;
}

// 获取订单状态
// 返回订单状态枚举值
Order::orderStatus Order::getOrderStatus() const {
    return order_status;
}

// 获取订单商品列表
// 返回包含商品名称和数量对的向量
std::vector<std::pair<std::string, int>> Order::getItems() const {
    return items;
}

// 获取订单总金额
// 返回订单总金额
double Order::getTotal() const {
    return total;
}

// 获取收货地址
// 返回收货地址字符串
std::string Order::getAddress() const {
    return address;
}

// Setters
// 设置订单状态
// status: 新的订单状态枚举值
void Order::setOrderStatus(orderStatus status) {
    this->order_status = status;
}

// 静态方法：格式化时间点为 UTC 时间字符串
// tp_s: 要格式化的时间点
// 返回格式化后的 UTC 时间字符串
std::string Order::format_timepoint_utc_seconds(const sys_seconds_tp& tp_s) {
    return std::format("{:%Y-%m-%d %H:%M:%S} UTC", tp_s);
}

// 静态方法：格式化时间点为本地时间字符串
// tp_s: 要格式化的时间点
// 返回格式化后的本地时间字符串
std::string Order::format_timepoint_local_seconds(const sys_seconds_tp& tp_s) {
    // 将时间点转换为带时区的时间
    auto zt = std::chrono::zoned_time(std::chrono::current_zone(), tp_s);
    // 格式化时间为字符串，包含时区信息
    return std::format("{:%Y-%m-%d %H:%M:%S %Z}", zt);
}

// 将订单信息转换为字符串表示
// 返回订单信息的字符串
std::string Order::toString() const {
    // 构建订单信息字符串，包括订单ID
    std::string result = "Order ID: " + this->orderId + "\t\t";

    // 添加订单时间和订单状态
    result += "Time: " + format_timepoint_local_seconds(this->time) + "\t";
    result += "Order Status: ";
    // 根据订单状态枚举值添加对应的状态字符串
    switch (this->order_status) {
    case PENDING:
        result += "Pending"; // 待处理
        break;
    case SHIPPED:
        result += "Shipped"; // 已发货
        break;
    case DELIVERED:
        result += "Delivered"; // 已送达
        break;
    case CANCELLED:
        result += "Cancelled"; // 已取消
        break;
    default: ; // 默认情况
    }
    result += "\t";
    // 添加下单用户信息
    result += "User: " + this->orderUser;
    result += "\nItems:\n"; // 添加商品列表头部
    // 遍历订单商品列表，添加每个商品的名称和数量
    for (const auto& [fst, snd] : this->items) {
        result += "\t- Name: " + fst + ", Quantity: " + std::to_string(snd) + "\n";
    }
    // 添加订单总金额和收货地址
    result += "Total: $" + std::to_string(this->total) + "\t";
    result += "Address: " + this->address + "\n";
    // 返回完整的订单信息字符串
    return result;
}

// 获取下单用户
// 返回下单用户字符串
std::string Order::getOrderUser() const {
    return orderUser;
}

// 设置订单收货地址
// address: 新的收货地址字符串
void Order::setOrderAddress(const std::string& address) {
    this->address = address;
}

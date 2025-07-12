//
// Created by liu on 4/8/25.
//

#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <map>
#include <utility>
#include "coupon.h"
#include <tuple>

// 产品类，表示一个具体的产品
class Product {
public:
    // 产品信息结构体
    typedef struct {
        std::string name; // 产品名称
        std::string description; // 产品描述
        double price; // 产品价格
        int inventory; // 产品库存
        std::string category; // 产品类别
    } productInfo;

    // 构造函数，使用产品信息结构体初始化产品
    explicit Product(productInfo info) : info(std::move(info)) {};
    // 默认构造函数
    Product() = default;

    // 获取产品名称
    [[nodiscard]] std::string getName() const {
        return info.name;
    }

    // 获取产品描述
    [[nodiscard]] std::string getDescription() const {
        return info.description;
    }

    // 获取产品价格
    [[nodiscard]] double getPrice() const {
        return info.price;
    }

    // 获取产品库存
    [[nodiscard]] int getInventory() const {
        return info.inventory;
    }

    // 获取产品类别
    [[nodiscard]] std::string getCategory() const {
        return info.category;
    }

    // 设置产品名称
    void setName(const std::string& name) {
        info.name = name;
    }

    // 设置产品描述
    void setDescription(const std::string& description) {
        info.description = description;
    }

    // 设置产品价格
    void setPrice(const double price) {
        info.price = price;
    }

    // 设置产品库存
    void setInventory(const int inventory) {
        info.inventory = inventory;
    }

    // 设置产品类别
    void setCategory(const std::string& category) {
        info.category = category;
    }

    // 设置产品信息
    void setProductInfo(const productInfo& info) {
        this->info = info;
    }

    // 减少产品库存
    // quantity: 要减少的数量
    void decreaseInventory(const int quantity) {
        info.inventory -= quantity;
    }

    // 增加产品库存
    // quantity: 要增加的数量
    void increaseInventory(const int quantity) {
        info.inventory += quantity;
    }

    // 获取产品信息结构体
    [[nodiscard]] productInfo getProductInfo() const {
        return info;
    }

    // 将产品信息转换为字符串表示（包含优惠信息）
    // coupon: 优惠券对象引用，用于获取优惠信息
    // 返回产品的详细信息字符串
    [[nodiscard]] std::string toString(Coupon& coupon) const {
        // 获取适用于该产品的优惠券折扣比例、开始时间和结束时间
        const std::tuple<double, std::string, std::string> ratio = coupon.getCouponDiscount(info.name, info.category);

        std::stringstream ss; // 用于格式化价格的字符串流
        // 将应用折扣后的价格格式化到 ss 字符串流中，保留两位小数
        ss << std::fixed << std::setprecision(2) << info.price * std::get<0>(ratio);

        std::stringstream ss2; // 用于格式化折扣百分比的字符串流
        // 将折扣百分比格式化到 ss2 字符串流中，保留零位小数
        ss2 << std::fixed << std::setprecision(0) << 100 * (1 - std::get<0>(ratio));

        // 构建最终的产品信息字符串
        return "Name: " + info.name + "\nDescription: " + info.description +
            "\nPrice: " + ss.str() + "\nInventory: " + std::to_string(info.inventory) +
            "\nCategory: " + info.category + "\nDiscount: " + ss2.str() + "%\n" // 折扣百分比
            "Start Time: " + std::get<1>(ratio) + "\nEnd Time: " + std::get<2>(ratio) + "\n"; // 优惠券有效期
    }


    // 将产品信息转换为简略字符串表示（包含优惠后价格）
    // coupon: 优惠券对象引用，用于计算应用优惠后的价格
    // 返回产品的简略信息字符串
    [[nodiscard]] std::string toStringShort(Coupon&) const;

private:
    productInfo info; // 存储产品信息
};


#endif //PRODUCT_H

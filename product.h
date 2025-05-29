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

class Product {
public:
    typedef struct {
        std::string name;
        std::string description;
        double price;
        int inventory;
        std::string category;
    } productInfo;

    explicit Product(productInfo info) : info(std::move(info)) {};
    Product() = default;

    [[nodiscard]] std::string getName() const {
        return info.name;
    }

    [[nodiscard]] std::string getDescription() const {
        return info.description;
    }

    [[nodiscard]] double getPrice() const {
        return info.price;
    }

    [[nodiscard]] int getInventory() const {
        return info.inventory;
    }

    [[nodiscard]] std::string getCategory() const {
        return info.category;
    }

    void setName(const std::string& name) {
        info.name = name;
    }

    void setDescription(const std::string& description) {
        info.description = description;
    }

    void setPrice(const double price) {
        info.price = price;
    }

    void setInventory(const int inventory) {
        info.inventory = inventory;
    }

    void setCategory(const std::string& category) {
        info.category = category;
    }

    void setProductInfo(const productInfo& info) {
        this->info = info;
    }

    void decreaseInventory(const int quantity) {
        info.inventory -= quantity;
    }

    void increaseInventory(const int quantity) {
        info.inventory += quantity;
    }

    [[nodiscard]] productInfo getProductInfo() const {
        return info;
    }

    [[nodiscard]] std::string toString(const Coupon& coupon) const {
        const std::tuple<double, std::string, std::string> ratio = coupon.getCouponDiscount(info.name, info.category);

        std::stringstream ss;
        ss << std::fixed << std::setprecision(2) << info.price * std::get<0>(ratio);

        std::stringstream ss2;
        ss2 << std::fixed << std::setprecision(0) << 100 * (1 - std::get<0>(ratio));

        return "Name: " + info.name + "\nDescription: " + info.description +
            "\nPrice: " + ss.str() + "\nInventory: " + std::to_string(info.inventory) +
            "\nCategory: " + info.category + "\nDiscount: " + ss2.str() + "%\n"
            "Start Time: " + std::get<1>(ratio) + "\nEnd Time: " + std::get<2>(ratio) + "\n";
    }


    [[nodiscard]] std::string toStringShort(const Coupon&) const;

private:
    productInfo info;
};


#endif //PRODUCT_H

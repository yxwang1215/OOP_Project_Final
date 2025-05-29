//
// Created by liu on 4/8/25.
//

#ifndef PRODUCTLIST_H
#define PRODUCTLIST_H

#include "product.h"
#include "coupon.h"

class ProductList {
public:
    explicit ProductList();
    void printList(const Coupon&) const;
    void addProduct(const std::string&, const std::string&, double, int, const std::string& category);
    void writeToFile() const;
    void editProduct(const std::string&, const std::string&, double, int, const std::string& category);
    void decreaseInventory(const std::string& name, int quantity);
    void increaseInventory(const std::string& name, int quantity);
    [[nodiscard]] bool hasProduct(const std::string& name) const;
    void deleteProduct(const std::string& name);
    void searchProduct(const std::string& name, const Coupon&) const;
    void fuzzySearchProduct(const std::string& name, const Coupon&) const;
    [[nodiscard]] std::string getCategory(const std::string& name) const;
    [[ nodiscard ]] std::string getDescription(const std::string& name) const;
    [[ nodiscard ]] double getPrice(const std::string& name) const;
    [[ nodiscard ]] int getInventory(const std::string& name) const;

private:
    std::map<std::string, Product> productMap;
    std::string fileName;
};


#endif //PRODUCTLIST_H

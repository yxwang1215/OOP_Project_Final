//
// Created by liu on 6/1/25.
//
// 统计信息实现文件

#include "statistics.h"
#include "orderList.h"
#include "productList.h"
#include "order.h"
#include <string>
#include <vector>
#include <map>
#include <fstream>

#include "log.h"

// 打印统计信息
void Statistics::printStatistics() const {

    // 存储每个用户购买的每个产品的统计信息 (用户名, 产品名) -> (数量, 总价格)
    std::map<std::pair<std::string, std::string>, std::pair<int, double>> productStatistics;
    // 存储每个用户购买的每个类别的统计信息 (用户名, 类别) -> (数量, 总价格)
    std::map<std::pair<std::string, std::string>, std::pair<int, double>> categoryStatistics;

    // 遍历所有订单
    for (auto &order : orderList->getAllOrders()) {
        // 获取订单用户名
        const std::string username = order.getOrderUser();
        // 遍历订单中的所有商品项
        for (const std::vector<std::pair<std::string, int>> items = order.getItems(); const auto & [fst, snd] : items) {
            // 获取产品名称
            const std::string productName = fst;
            // 获取产品类别
            const std::string category = productList->getCategory(productName);
            // 获取产品价格
            const double price = productList->getPrice(productName);
            // 获取购买数量
            const int quantity = snd;

            // 更新产品统计信息
            if (productStatistics.contains({username, productName})) {
                // 如果已存在该用户的该产品记录，则累加数量和总价格
                productStatistics[{username, productName}] = {productStatistics[{username, productName}].first + quantity, productStatistics[{username, productName}].second + price * quantity};
            }
            else {
                // 如果不存在，则创建新的记录
                productStatistics[{username, productName}] = {quantity, price * quantity};
            }

            // 更新类别统计信息
            if (categoryStatistics.contains({username, category})) {
                // 如果已存在该用户的该类别记录，则累加数量和总价格
                categoryStatistics[{username, category}] = {categoryStatistics[{username, category}].first + quantity, categoryStatistics[{username, category}].second + price * quantity};
            }
            else {
                // 如果不存在，则创建新的记录
                categoryStatistics[{username, category}] = {quantity, price * quantity};
            }
        }
    }

    // 将统计信息写入 statistics.csv 文件
    if (std::ofstream statisticsFile("statistics.csv"); statisticsFile.is_open()) {
        // 写入 CSV 文件头
        statisticsFile << "username, product, category, quantity, price" << std::endl;
        // 写入产品统计信息
        for (const auto & [fst, snd] : productStatistics) {
            statisticsFile << fst.first << ", " << fst.second << ", " << productList->getCategory(fst.second) << ", " << snd.first << ", " << snd.second << std::endl;
        }
        // 写入类别统计信息
        for (const auto & [fst, snd] : categoryStatistics) {
            statisticsFile << fst.first << ", -, " << fst.second << ", " << snd.first << ", " << snd.second << std::endl;
        }
        // 关闭文件
        statisticsFile.close();
        // 记录统计信息写入成功日志
        logger << true << LogString::StatisticsWriteSuccess;
    }
}

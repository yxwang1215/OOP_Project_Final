//
// Created by liu on 5/22/25.
//

#include "coupon.h"
#include <fstream>
#include <ranges>
#include <tuple>
#include <chrono>
#include "log.h"

// 优惠券类构造函数
// 从 coupon.txt 文件加载优惠券信息
Coupon::Coupon() {
    // 打开 coupon.txt 文件
    std::ifstream couponFile("coupon.txt");
    // 检查文件是否成功打开
    if (couponFile.is_open()) {
        // 读取满减优惠的最低消费金额和折扣金额
        couponFile >> minimum >> discount;
        std::string name, category;
        long long startTime_, endTime_;
        double ratio;
        // 循环读取每个优惠券的信息
        while (couponFile >> name >> category >> startTime_ >> endTime_ >> ratio) {
            couponInfo info;
            couponName couponName;
            // 设置优惠券名称、类别、开始时间、结束时间和折扣比例
            info.name = name;
            info.category = category;
            info.startTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(
                std::chrono::seconds(startTime_));
            info.endTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(
                std::chrono::seconds(endTime_));
            info.ratio = ratio;
            couponName.name = name;
            couponName.category = category;
            // 将优惠券信息存储到 map 中
            couponMap[couponName] = info;
        }
    }
    // 关闭文件
    couponFile.close();
    // 清除已过期的优惠券
    erase_expired_coupon();
}

// 清除已过期的优惠券
void Coupon::erase_expired_coupon() {
    // 获取当前时间
    const auto now = std::chrono::system_clock::now();
    // 遍历优惠券 map
    for (auto it = couponMap.begin(); it != couponMap.end();) {
        // 如果优惠券的结束时间早于当前时间，则表示已过期
        if (it->second.endTime < now) {
            // 移除该优惠券
            it = couponMap.erase(it);
        } else {
            // 否则，移动到下一个优惠券
            ++it;
        }
    }
}


// 将优惠券信息写入文件
// 将优惠券信息写入文件
void Coupon::writeToFile() {
    // 清除已过期的优惠券
    erase_expired_coupon();
    // 打开 coupon.txt 文件进行写入
    std::ofstream couponFile("coupon.txt");
    // 检查文件是否成功打开
    if (couponFile.is_open()) {
        // 写入满减优惠的最低消费金额和折扣金额
        couponFile << minimum << " " << discount << std::endl;
        // 遍历优惠券 map 中的所有优惠券信息
        for (const auto& [name, category, startTime, endTime, ratio] : couponMap | std::views::values) {
            // 将每个优惠券的名称、类别、开始时间（秒）、结束时间（秒）和折扣比例写入文件
            couponFile << name << " " << category << " " << std::chrono::duration_cast<std::chrono::seconds>(
                    startTime.time_since_epoch()).count() << " "
                << std::chrono::duration_cast<std::chrono::seconds>(endTime.time_since_epoch()).count() << " " << ratio
                << std::endl;
        }
    }
    // 关闭文件
    couponFile.close();
}

// 获取满减优惠的最低消费金额
// 获取满减优惠的最低消费金额
double Coupon::getMinimum() const {
    // 返回最低消费金额
    return minimum;
}

// 获取满减优惠的折扣金额
// 获取满减优惠的折扣金额
double Coupon::getDiscount() const {
    // 返回折扣金额
    return discount;
}

// 获取所有优惠券的 map
// 获取所有优惠券的 map
std::map<Coupon::couponName, Coupon::couponInfo> Coupon::getCouponMap() {
    // 清除已过期的优惠券
    erase_expired_coupon();
    // 返回优惠券 map
    return couponMap;
}

// 获取指定商品或类别的优惠券折扣信息
// name: 商品名称
// category: 商品类别
// 返回一个 tuple，包含折扣比例、开始时间和结束时间
// 获取指定商品或类别的优惠券折扣信息
// name: 商品名称
// category: 商品类别
// 返回一个 tuple，包含折扣比例、开始时间和结束时间
std::tuple<double, std::string, std::string> Coupon::getCouponDiscount(const std::string& name, const std::string& category) {
    // 清除已过期的优惠券
    erase_expired_coupon();
    Coupon::couponName couponName;
    // 尝试按商品名称查找优惠券
    couponName.name = name;
    couponName.category = "-"; // 类别设置为 "-" 表示不限制类别
    if (couponMap.contains(couponName)) {
        // 如果找到按商品名称匹配的优惠券，返回其折扣信息
        return std::make_tuple(couponMap.at(couponName).ratio, format_timepoint_local_seconds(couponMap.at(couponName).startTime), format_timepoint_local_seconds(couponMap.at(couponName).endTime));
    }
    // 如果没有找到按商品名称匹配的优惠券，尝试按类别查找
    couponName.name = "-"; // 名称设置为 "-" 表示不限制名称
    couponName.category = category;
    if (couponMap.contains(couponName)) {
        // 如果找到按类别匹配的优惠券，返回其折扣信息
        return std::make_tuple(couponMap.at(couponName).ratio, format_timepoint_local_seconds(couponMap.at(couponName).startTime), format_timepoint_local_seconds(couponMap.at(couponName).endTime));
    }
    // 如果没有找到匹配的优惠券，返回默认折扣信息（无折扣，当前时间作为开始和结束时间）
    return std::make_tuple(1.0, format_timepoint_local_seconds(std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())), format_timepoint_local_seconds(std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())));
}



// 添加新的优惠券
// name: 优惠券名称
// category: 优惠券类别
// discount: 折扣比例
// daysToAdd: 优惠券有效天数
// 添加新的优惠券
// name: 优惠券名称
// category: 优惠券类别
// discount: 折扣比例
// daysToAdd: 优惠券有效天数
void Coupon::addCoupon(const std::string& name, const std::string& category, const double discount, const int daysToAdd) {
    Coupon::couponName couponName;
    // 设置优惠券名称和类别
    couponName.name = name;
    couponName.category = category;
    couponInfo info;
    // 设置优惠券信息
    info.name = name;
    info.category = category;
    // 设置优惠券开始时间为当前时间
    info.startTime = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    // 计算优惠券结束时间
    const auto durationToAdd = std::chrono::seconds(daysToAdd); // C++20
    info.endTime = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now() + durationToAdd);
    // 设置折扣比例
    info.ratio = discount;
    // 将新的优惠券添加到 map 中
    couponMap[couponName] = info;
    // 将更新后的优惠券信息写入文件
    writeToFile();
}

// 设置满减优惠的折扣金额
// dis: 折扣金额
// 设置满减优惠的折扣金额
// dis: 折扣金额
void Coupon::setDiscount(const double dis) {
    // 更新折扣金额
    discount = dis;
}

// 设置满减优惠的最低消费金额
// min: 最低消费金额
// 设置满减优惠的最低消费金额
// min: 最低消费金额
void Coupon::setMinimum(const double min) {
    // 更新最低消费金额
    minimum = min;
}

// 设置满减优惠的最低消费金额和折扣金额
// min: 最低消费金额
// dis: 折扣金额
// 如果设置成功返回 true，否则返回 false
// 设置满减优惠的最低消费金额和折扣金额
// min: 最低消费金额
// dis: 折扣金额
// 如果设置成功返回 true，否则返回 false
bool Coupon::setMJDiscount(const double min, const double dis) {
    // 检查最低消费金额和折扣金额是否有效
    if (min > 0 && dis > 0 && dis < min) {
        // 更新最低消费金额和折扣金额
        minimum = min;
        discount = dis;
        // 将更新后的信息写入文件
        writeToFile();
        // 记录成功日志
        logger << true << LogString::AddThresholdCouponSuccess;
        // 返回 true 表示设置成功
        return true;
    }
    // 记录失败日志
    logger << true << LogString::AddThresholdCouponFail;
    // 返回 false 表示设置失败
    return false;
}

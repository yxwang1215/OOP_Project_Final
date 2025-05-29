//
// Created by liu on 5/22/25.
//

#include "coupon.h"
#include <fstream>
#include <ranges>
#include <tuple>
#include <chrono>
#include "log.h"

Coupon::Coupon() {
    std::ifstream couponFile("coupon.txt");
    if (couponFile.is_open()) {
        couponFile >> minimum >> discount;
        std::string name, category;
        long long startTime_, endTime_;
        double ratio;
        while (couponFile >> name >> category >> startTime_ >> endTime_ >> ratio) {
            couponInfo info;
            couponName couponName;
            info.name = name;
            info.category = category;
            info.startTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(
                std::chrono::seconds(startTime_));
            info.endTime = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>(
                std::chrono::seconds(endTime_));
            info.ratio = ratio;
            couponName.name = name;
            couponName.category = category;
            couponMap[couponName] = info;
        }
    }
    couponFile.close();
}

void Coupon::writeToFile() const {
    std::ofstream couponFile("coupon.txt");
    if (couponFile.is_open()) {
        couponFile << minimum << " " << discount << std::endl;
        for (const auto& [name, category, startTime, endTime, ratio] : couponMap | std::views::values) {
            couponFile << name << " " << category << " " << std::chrono::duration_cast<std::chrono::seconds>(
                    startTime.time_since_epoch()).count() << " "
                << std::chrono::duration_cast<std::chrono::seconds>(endTime.time_since_epoch()).count() << " " << ratio
                << std::endl;
        }
    }
    couponFile.close();
}

double Coupon::getMinimum() const {
    return minimum;
}

double Coupon::getDiscount() const {
    return discount;
}

std::map<Coupon::couponName, Coupon::couponInfo> Coupon::getCouponMap() const {
    return couponMap;
}

std::tuple<double, std::string, std::string> Coupon::getCouponDiscount(const std::string& name, const std::string& category) const {
    Coupon::couponName couponName;
    couponName.name = name;
    couponName.category = "-";
    if (couponMap.contains(couponName)) {
        return std::make_tuple(couponMap.at(couponName).ratio, format_timepoint_local_seconds(couponMap.at(couponName).startTime), format_timepoint_local_seconds(couponMap.at(couponName).endTime));
    }
    couponName.name = "-";
    couponName.category = category;
    if (couponMap.contains(couponName)) {
        return std::make_tuple(couponMap.at(couponName).ratio, format_timepoint_local_seconds(couponMap.at(couponName).startTime), format_timepoint_local_seconds(couponMap.at(couponName).endTime));
    }
    return std::make_tuple(1.0, format_timepoint_local_seconds(std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())), format_timepoint_local_seconds(std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now())));
}



void Coupon::addCoupon(const std::string& name, const std::string& category, const double discount, const int daysToAdd) {
    Coupon::couponName couponName;
    couponName.name = name;
    couponName.category = category;
    couponInfo info;
    info.name = name;
    info.category = category;
    info.startTime = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now());
    const auto durationToAdd = std::chrono::days(daysToAdd); // C++20
    info.endTime = std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now() + durationToAdd);
    info.ratio = discount;
    couponMap[couponName] = info;
    writeToFile();
}

void Coupon::setDiscount(const double dis) {
    discount = dis;
}

void Coupon::setMinimum(const double min) {
    minimum = min;
}

bool Coupon::setMJDiscount(const double min, const double dis) {
    if (min > 0 && dis > 0 && dis < min) {
        minimum = min;
        discount = dis;
        writeToFile();
        logger << true << LogString::AddThresholdCouponSuccess;
        return true;
    }
    logger << true << LogString::AddThresholdCouponFail;
    return false;
}

//
// Created by liu on 5/22/25.
//

#ifndef COUPON_H
#define COUPON_H
#include <map>
#include <string>
#include <chrono>

using sys_seconds_tp = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;


class Coupon {
public:
    typedef struct couponInfo {
        std::string name;
        std::string category;
        sys_seconds_tp startTime;
        sys_seconds_tp endTime;
        double ratio;
    } couponInfo;

    typedef struct couponName {
        std::string name;
        std::string category;
        bool operator < (const couponName& other) const {
            return name < other.name || (name == other.name && category < other.category);
        }
    } couponName;

    Coupon();

    //getters
    [[nodiscard]] double getMinimum() const;
    [[nodiscard]] double getDiscount() const;
    [[nodiscard]] std::map<couponName, couponInfo> getCouponMap() const;
    [[nodiscard]] std::tuple<double, std::string, std::string> getCouponDiscount(const std::string& name, const std::string& category) const;

    //setters
    void setMinimum(double min);
    void setDiscount(double dis);
    void addCoupon(const std::string& name, const std::string& category, double discount, int);

    void writeToFile() const;

    bool setMJDiscount(double min, double dis);

    static std::string format_timepoint_local_seconds(const sys_seconds_tp& tp_s) {
        auto zt = std::chrono::zoned_time(std::chrono::current_zone(), tp_s);
        return std::format("{:%Y-%m-%d %H:%M:%S %Z}", zt);
    }

private:
    double minimum = 0;
    double discount = 0;
    std::map<couponName, couponInfo> couponMap;
};


#endif //COUPON_H

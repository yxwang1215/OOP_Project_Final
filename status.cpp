#include "status.h"
#include "user.h"
#include <iostream>
#include <format>
#include "log.h"
#include "productList.h"

Status::Status() {
    this->status = NOT_LOGIN;
    this->user = User();
    this->admin = Admin();
    this->currentUser = "";
    this->currentUserBase = nullptr;
    this->productList = ProductList();
    this->coupon = Coupon();
}

bool Status::login(const std::string& username, const std::string& password) {
    auto res = this->user.login(username, password);
    if (res != UserBase::ERROR) {
        this->status = LOGIN;
        this->currentUser = username;
        this->currentUserBase = user.getBasePtr();
        return true;
    }
    res = this->admin.login(username, password);
    if (res != UserBase::ERROR) {
        this->status = ADMIN;
        this->currentUser = username;
        this->currentUserBase = admin.getBasePtr();
        return true;
    }
    return false;
}

void Status::logout() {
    this->status = NOT_LOGIN;
    this->currentUser = "";
    this->currentUserBase = nullptr;
}

bool Status::changePassword(const std::string& password) const {
    return this->currentUserBase->changePassword(currentUser, password);
}

Status::StatusType Status::getStatus() const {
    return status;
}

std::string Status::getCurrentUser() const {
    return currentUser;
}

bool Status::registerUser(const std::string& username, const std::string& password) {
    return this->user.registerUser(username, password);
}

UserBase* Status::getUser() const {
    return this->currentUserBase;
}

void Status::printList() const {
    this->printMJCoupon();
    this->productList.printList(this->coupon);
}

void Status::addProduct(const std::string& name, const std::string& description, double price, int inventory,
                        const std::string& category) {
    if (this->status == ADMIN) {
        Admin::addProduct(name, description, price, inventory, category, this->productList);
    }
    else {
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

void Status::editProduct(const std::string& name, const std::string& description, const double price,
                         const int inventory,
                         const std::string& category) {
    if (this->status == ADMIN) {
        Admin::editProduct(name, description, price, inventory, category, this->productList);
    }
    else {
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

bool Status::hasProduct(const std::string& name) const {
    return this->productList.hasProduct(name);
}

void Status::deleteProduct(const std::string& name) {
    if (this->status == ADMIN) {
        Admin::deleteProduct(name, this->productList);
    }
    else {
        // Log::printLog(LogString::PermissionDenied, true);
        logger << true << LogString::PermissionDenied;
    }
}

void Status::searchProduct(const std::string& name) const {
    this->productList.searchProduct(name, coupon);
}

void Status::fuzzySearchProduct(const std::string& name) const {
    this->productList.fuzzySearchProduct(name, coupon);
}

void Status::addCart(const std::string& name, const int quantity) {
    if (quantity <= 0) {
        logger << true << LogString::Invalid;
        return;
    }
    if (!productList.hasProduct(name)) {
        logger << true << LogString::ProductNotExist;
        return;
    }
    if (this->status == LOGIN) {
        this->user.addCart(currentUser, name, quantity);
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::editCart(const std::string& name, const int quantity) {
    if (quantity <= 0) {
        logger << true << LogString::WantToDelete;
        std::string op;
        std::cin >> op;
        if (op == "y" || op == "Y") {
            this->deleteCart(name);
            return;
        }
        logger << true;
        return;
    }
    if (this->status == LOGIN) {
        this->user.editCart(currentUser, name, quantity);
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::deleteCart(const std::string& name) {
    if (this->status == LOGIN) {
        this->user.deleteCart(currentUser, name);
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::clearCart() {
    if (this->status == LOGIN) {
        this->user.clearCart(currentUser);
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::printCart() {
    if (this->status == LOGIN) {
        double sumprice = 0;
        int sumqua = 0;
        auto cart = this->user.getCartList(currentUser);
        logger << true << LogString::CartHead;
        for (const auto& [name, quantity] : cart) {
            logger << Log::paddingEnd(15, name) << Log::paddingEnd(10, productList.getCategory(name)) <<
                Log::paddingEnd(20, productList.getDescription(name)) << Log::paddingEnd(
                    10, std::format("{:.2f}", productList.getPrice(name))) << Log::paddingEnd(
                    10, std::to_string(quantity)) << "\n";
            sumqua += quantity;
            sumprice += productList.getPrice(name) * quantity;
        }
        logger << LogString::CartTail << LogString::SumQua << sumqua << "\t" << LogString::SumPrice << sumprice << "\n"
            << LogString::ExitList;
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

bool Status::addOrder(const std::vector<std::pair<std::string, int>>& items, const std::string& address) {
    if (this->status == LOGIN) {
        bool enough = true;
        for (const auto& [name, quantity] : items) {
            if (!this->productList.hasProduct(name)) {
                logger << true << LogString::ProductNotExist;
                enough = false;
                break;
            }
            if (this->productList.getInventory(name) < quantity) {
                logger << true << LogString::ProductNotEnough;
                logger << true << LogString::ProductNotEnough;
                enough = false;
                break;
            }
        }
        if (enough) {
            if (orderList.addOrder(this->currentUser, items, address, coupon)) {
                for (const auto& [name, quantity] : items) {
                    this->productList.decreaseInventory(name, quantity);
                }
                logger << true << LogString::AddOrderSuccess;
                return true;
            }
        }
        logger << true;
        return false;
    }
    logger << true << LogString::PermissionDenied;
    return false;
}

void Status::addOrderFromCart(const std::string& address) {
    if (this->status == LOGIN) {
        if (this->user.getCartList(this->currentUser).empty()) {
            logger << true << LogString::EmptyCart;
            return;
        }
        if (addOrder(this->user.getCartList(this->currentUser), address)) {
            this->user.clearCart(this->currentUser);
        }
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::deleteOrder(const std::string& orderId) {
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        if (!this->orderList.hasOrder(orderId)) {
            logger << true << LogString::OrderNotExist;
            return;
        }
        if (this->orderList.getOrderStatus(orderId) == Order::DELIVERED) {
            orderList.deleteOrder(orderId);
            logger << true << LogString::DeleteOrderSuccess;
        }
        else {
            logger << true << LogString::DeleteOrderFail;
        }
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::changeOrderStatus(const std::string& orderId, const Order::orderStatus status) {
    if (this->status == ADMIN) {
        if (!this->orderList.hasOrder(orderId)) {
            logger << true << LogString::OrderNotExist;
            return;
        }
        this->orderList.changeOrderStatus(orderId, status, 0);
        logger << true << LogString::ChangeOrderStatusSuccess;
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::cancelOrder(const std::string& orderId) {
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        if (!this->orderList.hasOrder(orderId)) {
            logger << true << LogString::OrderNotExist;
            return;
        }
        if (this->orderList.getOrderStatus(orderId) == Order::PENDING) {
            this->orderList.changeOrderStatus(orderId, Order::CANCELLED, 0);
            for (const auto& [name, quantity] : this->orderList.getOrderItems(orderId)) {
                this->productList.increaseInventory(name, quantity);
            }
            logger << true << LogString::CancelOrderSuccess;
        }
        else {
            logger << true << LogString::CancelOrderFail;
        }
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::printOrder() {
    if (this->status == LOGIN) {
        const auto order = this->orderList.getOrderList(this->currentUser);
        logger << true << LogString::OrderHead;
        for (const auto& orderId : order) {
            orderList.printOrder(orderId);
            logger << LogString::Hr;
        }
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::changeOrderAddress(const std::string& orderId, const std::string& address) {
    if (this->status == LOGIN && this->currentUser == this->orderList.getOrderUser(orderId)) {
        if (!this->orderList.hasOrder(orderId)) {
            logger << true << LogString::OrderNotExist;
            return;
        }
        if (this->orderList.getOrderStatus(orderId) == Order::PENDING) {
            if (orderList.setOrderAddress(orderId, address)) {
                logger << true << LogString::EditAddressSuccess;
            }
            else {
                logger << true << LogString::EditAddressFail;
            }
        }
        else {
            logger << true << LogString::EditAddressFail;
        }
    }
}

void Status::printAllOrders() const {
    if (this->status == ADMIN) {
        logger << true;
        orderList.printAllOrders();
        logger << LogString::ExitList;
    }
    else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::printMJCoupon() const {
    logger << true;
    const double mini = this->coupon.getMinimum();
    if (const double disc = this->coupon.getDiscount(); mini != 0 && disc != 0) {
        logger << "spend " << mini << " or more to get " << disc << " off\n";
    }
}

void Status::add_coupon(const std::string& name, const std::string& category, double discount, int daysToAdd) {
    if (this->status == ADMIN) {
        this->coupon.addCoupon(name, category, discount, daysToAdd);
        logger << true << LogString::AddPercentCouponSuccess;
    }else {
        logger << true << LogString::PermissionDenied;
    }
}

void Status::setMJCoupon(const double minimum, const double discount) {
    if (this->status == ADMIN) {
        this->coupon.setMJDiscount(minimum, discount);
    }
}

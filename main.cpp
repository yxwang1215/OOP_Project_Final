#include <iostream>
#include <string>
#include "log.h"
#include "status.h"
#include <regex>
using namespace std;

void adminFunc(Status& status) {
    string option;
    bool is_first = true;
    while (true) {
        // Log::printLog(LogString::AdminPanel, is_first);
        logger << is_first << LogString::AdminPanel;
        is_first = false;
        cin >> option;
        if (option == "0") {
            return;
        }
        else if (option == "1") {
            logger << true << LogString::ChangePassword;
            string password;
            cin >> password;
            if (status.changePassword(password)){};
        }
        else if (option == "2") {
            string name, description, category;
            double price;
            int inventory;
            logger << true << LogString::InputProductName;
            cin >> name;
            if (status.hasProduct(name)) {
                logger << true << LogString::ProductExist;
                continue;
            }
            logger << LogString::InputProductDescription;
            cin.ignore();
            getline(cin, description);
            logger << LogString::InputProductPrice;

            while (cin >> price, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::InputProductPrice;
            }
            logger << LogString::InputProductInventory;
            while (cin >> inventory, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::InputProductInventory;
            }
            logger << LogString::InputProductCategory;
            cin >> category;
            status.addProduct(name, description, price, inventory, category);
        }
        else if (option == "3") {
            status.printList();
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "4") {
            string name, description, category;
            double price;
            int inventory;
            // Log::printLog(LogString::EditProduct, true);
            logger << true << LogString::EditProduct;
            cin >> name;
            if (!status.hasProduct(name)) {
                // Log::printLog(LogString::ProductNotFound, true);
                logger << true << LogString::ProductNotFound;
                continue;
            }
            Log::printLog(LogString::EditProductDesc, false);
            cin.ignore();
            getline(cin, description);
            Log::printLog(LogString::EditProductPrice, false);
            while (cin >> price, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::EditProductPrice;
            }
            Log::printLog(LogString::EditProductInventory, false);
            while (cin >> inventory, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::EditProductInventory;
            }
            Log::printLog(LogString::EditProductCategory, false);
            cin >> category;
            status.editProduct(name, description, price, inventory, category);
        }
        else if (option == "5") {
            string name;
            // Log::printLog(LogString::DeleteProduct, true);
            logger << true << LogString::DeleteProduct;
            cin >> name;
            status.deleteProduct(name);
        }
        else if (option == "6") {
            logger << true << LogString::ChangeOrderStatus;
            string orderId;
            Order::orderStatus order_status = {};
            string order_status_str;
            cin >> orderId;
            logger << LogString::ChangeOrderStatusInputStatus;
            cin >> order_status_str;
            if (order_status_str == "pending") {
                order_status = Order::PENDING;
            }
            else if (order_status_str == "shipped") {
                order_status = Order::SHIPPED;
            }
            else if (order_status_str == "delivered") {
                order_status = Order::DELIVERED;
            }
            else if (order_status_str == "cancelled") {
                order_status = Order::CANCELLED;
            }
            status.changeOrderStatus(orderId, order_status);
        }
        else if (option == "7") {
            status.printAllOrders();
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "8") {
            logger << true << LogString::AddThresholdCouponMinimum;
            int threshold;
            while (cin >> threshold, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddThresholdCouponMinimum;
            }
            logger << true << LogString::AddThresholdCouponDiscount;
            double discount;
            while (cin >> discount, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddThresholdCouponDiscount;
            }
            status.setMJCoupon(threshold, discount);
        }
        else if (option == "9") {
            logger << true << LogString::AddPercentCouponName;
            string name;
            cin >> name;
            logger << true << LogString::AddPercentRatio;
            double discount;
            while (cin >> discount, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddPercentRatio;
            }
            int duration;
            logger << true << LogString::AddPercentDuration;
            while (cin >> duration, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddPercentDuration;
            }
            status.add_coupon(name, "-", discount, duration);
        }
        else if (option == "10") {
            logger << true << LogString::AddPercentCouponCategory;
            string name;
            cin >> name;
            logger << true << LogString::AddPercentRatio;
            double discount;
            while (cin >> discount, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddPercentRatio;
            }
            int duration;
            logger << true << LogString::AddPercentDuration;
            while (cin >> duration, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::AddPercentDuration;
            }
            status.add_coupon("-", name, discount, duration);
        }
        else {
            logger << true << LogString::Invalid;
        }
    }
}

void userOrderFunc(Status& status) {
    string option;
    bool is_first = true;
    while (true) {
        logger << is_first << LogString::UserOrderPanel;
        is_first = false;
        cin >> option;
        if (option == "0") {
            return;
        }
        if (option == "1") {
            logger << true << LogString::InputOrderName;
            string name;
            cin >> name;
            logger << LogString::InputOrderQua;
            int quantity;
            while (cin >> quantity, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::InputOrderQua;
            }
            string address;
            logger << LogString::InputOrderAddress;
            cin.ignore();
            getline(cin, address);
            status.addOrder({{name, quantity}}, address);
        }
        else if (option == "2") {
            status.printOrder();
            logger << LogString::ExitList;
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "3") {
            logger << true << LogString::ChangeAddressId;
            string OrderId;
            cin >> OrderId;
            string address;
            logger << LogString::InputOrderAddress;
            cin.ignore();
            getline(cin, address);
            status.changeOrderAddress(OrderId, address);
        }
        else if (option == "4") {
            logger << true << LogString::DeleteOrder;
            string OrderId;
            cin >> OrderId;
            status.deleteOrder(OrderId);
        }
        else if (option == "5") {
            logger << true << LogString::CancelOrder;
            string OrderId;
            cin >> OrderId;
            status.cancelOrder(OrderId);
        }
        else if (option == "6") {
            logger << true << LogString::CreateOrderFromCart;
            string op;
            cin >> op;
            if (op == "y" || op == "Y") {
                string address;
                logger << LogString::InputOrderAddress;
                cin.ignore();
                getline(cin, address);
                status.addOrderFromCart(address);
            }
        }
        else {
            logger << true << LogString::Invalid;
        }
    }
}

void userFunc(Status& status) {
    string option;
    bool is_first = true;
    while (true) {
        // Log::printLog(LogString::UserPanel, is_first);
        logger << is_first << LogString::UserPanel;
        is_first = false;
        cin >> option;
        if (option == "0") {
            return;
        }
        else if (option == "1") {
            // Log::printLog(LogString::ChangePassword, true);
            logger << true << LogString::ChangePassword;
            string password;
            cin >> password;
            status.changePassword(password);
        }
        else if (option == "2") {
            status.printList();
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "3") {
            string name;
            // Log::printLog(LogString::SearchProduct, true);
            logger << true << LogString::SearchProduct;
            cin >> name;
            status.searchProduct(name);
        }
        else if (option == "4") {
            string name;
            // Log::printLog(LogString::SearchProduct, true);
            logger << true << LogString::SearchProduct;
            cin >> name;
            status.fuzzySearchProduct(name);
        }
        else if (option == "5") {
            // list cart
            status.printCart();
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "6") {
            // add
            string name;
            int quantity;
            // Log::printLog(LogString::AddCart, true);
            logger << true << LogString::InputCartName;
            cin >> name;
            logger << LogString::InputCartQua;
            while (cin >> quantity, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::InputCartQua;
            }
            status.addCart(name, quantity);
        }
        else if (option == "7") {
            // edit
            string name;
            int quantity;
            // Log::printLog(LogString::EditCart, true);
            logger << true << LogString::EditCart;
            cin >> name;
            logger << LogString::EditCartQua;
            while (cin >> quantity, cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                logger << LogString::Invalid;
                logger << LogString::EditCartQua;
            }
            status.editCart(name, quantity);
        }
        else if (option == "8") {
            // delete
            string name;
            // Log::printLog(LogString::DeleteCart, true);
            logger << true << LogString::DeleteCart;
            cin.ignore();
            getline(cin, name);
            logger << true;
            const std::regex token_re(R"(\s*([^,]+?)\s*(?:,|$))");
            for (std::sregex_iterator it(name.begin(), name.end(), token_re), end; it != end; ++it) {
                status.deleteCart((*it)[1].str());
            }
        }
        else if (option == "9") {
            // order
            userOrderFunc(status);
            is_first = true;
        }
        else {
            // Log::printLog(LogString::Invalid, true);
            logger << true << LogString::Invalid;
        }
    }
}


int main() {
    // logger << "aaa";
    Status status;
    std::istream::sync_with_stdio(false);
    cin.tie(nullptr);
    string option;
    bool is_first = true;
    while (true) {
        logger << is_first << LogString::MENU;
        is_first = false;
        cin >> option;
        if (option == "0") {
            Log::printLog(LogString::Exit, true);
            logger << true << LogString::Exit;
            return 0;
        }
        if (option == "1") {
            bool is_first_2 = true;
            while (true) {
                logger << is_first_2 << LogString::Login;
                is_first_2 = false;
                string username, password;
                logger << LogString::Username;
                cin >> username;
                if (username == "-") {
                    is_first = true;
                    break;
                }
                logger << LogString::Password;
                cin >> password;
                if (status.login(username, password)) {
                    if (status.getStatus() == Status::ADMIN) {
                        adminFunc(status);
                    }
                    else if (status.getStatus() == Status::LOGIN) {
                        userFunc(status);
                    }
                    status.logout();
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "2") {
            logger << true << LogString::Register;
            string username, password;
            logger << LogString::Username;
            cin >> username;
            logger << LogString::Password;
            cin >> password;
            status.registerUser(username, password);
        }
        else if (option == "3") {
            status.printList();
            while (true) {
                string option_2;
                cin >> option_2;
                if (option_2 == "0") {
                    is_first = true;
                    break;
                }
            }
        }
        else if (option == "4") {
            string name;
            logger << true << LogString::SearchProduct;
            cin >> name;
            status.searchProduct(name);
        }
        else if (option == "5") {
            string name;
            logger << true << LogString::SearchProduct;
            cin >> name;
            status.fuzzySearchProduct(name);
        }
        else if (option == "6" || option == "7" || option == "8" || option == "9") {
            logger << true << LogString::NeedLogin;
        }
        else {
            logger << true << LogString::Invalid;
        }
    }
}

//
// Created by liu on 4/7/25.
//

#ifndef LOG_H
#define LOG_H
#include <string>

class Log {
public:
    static void printLog(const std::string&, bool clear);
    Log& operator<<(const std::string&);
    Log& operator<<(bool);
    Log& operator<<(const char*);
    Log& operator<<(char);
    Log& operator<<(int);
    Log& operator<<(double);
    static std::string paddingEnd(int len, const std::string&);
};

namespace LogString
{
    const std::string goodbye_art = R"(
  _____                 _ _
 / ____|               | | |
| |  __  ___   ___   __| | |__  _   _  ___
| | |_ |/ _ \ / _ \ / _` | '_ \| | | |/ _ \
| |__| | (_) | (_) | (_| | |_) | |_| |  __/
 \_____|\___/ \___/ \__,_|_.__/ \__, |\___|
                                 __/ |
                                |___/
    )";
    const std::string MENU = "welcome to use this system\n"
        "1. login\n"
        "2. register\n"
        "3. browse products\n"
        "4. search products\n"
        "5. fuzzy search products\n"
        "6. list cart\n"
        "7. add cart\n"
        "8. edit cart\n"
        "9. delete cart\n"
        "0. exit\n";
    const std::string Login = "please login\n"
        "please input your username and password(input - to exit)\n";
    const std::string Username = "username: ";
    const std::string Password = "password: ";
    const std::string changeLine = "\n";
    const std::string Exit = "thank you for using this system\n"
        "goodbye\n" + goodbye_art + "\n";

    const std::string WrongPassword = "\033[1;31musername or password is wrong\033[0m\n";
    const std::string Register = "please register\n"
        "please input your username and password\n";
    const std::string UserExist = "\033[1;31muser already exist\033[0m\n";
    const std::string UserNotExist = "\033[1;31muser not exist\033[0m\n";
    const std::string RegisterSuccess = "\033[1;32mregister success\033[0m\n";
    const std::string AdminPanel = "welcome to admin panel\n"
        "1. change password\n"
        "2. add product\n"
        "3. product list\n"
        "4. edit product\n"
        "5. delete product\n"
        "6. change order status\n"
        "7. order list\n"
        "8. add threshold coupon\n"
        "9. add percentage coupon by name\n"
        "10. add percentage coupon by category\n"
        "0. logout\n";
    const std::string ChangePassword = "please input your new password\n";
    const std::string ChangePasswordSuccess = "\033[1;32mchange password success\033[0m\n";
    const std::string FileNotFound = "\033[1;31mfile not found\033[0m\n";
    const std::string PasswordSame = "\033[1;31mpassword is the same\033[0m\n";
    const std::string UserPanel = "welcome to user panel\n"
        "1. change password\n"
        "2. product list\n"
        "3. search product\n"
        "4. fuzzy search product\n"
        "5. list cart\n"
        "6. add cart\n"
        "7. edit cart\n"
        "8. delete cart\n"
        "9. order\n"
        "0. logout\n";
    const std::string UserOrderPanel = "welcome to order panel\n"
        "1. create order\n"
        "2. order list\n"
        "3. edit order\n"
        "4. delete order\n"
        "5. cancel order\n"
        "6. create order from cart\n"
        "0. exit\n";
    const std::string BackendPanel = "welcome to backend panel\n"
        "Details not yet implemented\n"
        "0. exit\n";
    const std::string BuyPanel = "welcome to buy panel\n"
        "Details not yet implemented\n"
        "0. exit\n";

    const std::string ProductPanel = "welcome to Product List\n"
        "Details not yet implemented\n"
        "0. exit\n";
    const std::string Invalid = "\033[1;31minvalid input\033[0m\n";

    const std::string ProductHead = "Name                          category  desc                price     inv       \n"
                                    "--------------------------------------------------------------------------------\n";
    const std::string Authorization = "you are not authorized\n";
    const std::string PermissionDenied = "permission denied\n";
    const std::string InputProductName = "please input product name: ";
    const std::string InputProductCategory = "please input product category: ";
    const std::string InputProductDescription = "please input product description: ";
    const std::string InputProductPrice = "please input product price: ";
    const std::string InputProductInventory = "please input product inventory: ";
    const std::string ProductExist = "\033[1;31mproduct already exist\033[0m\n";
    const std::string AddProductSuccess = "\033[1;32madd product success\033[0m\n";
    const std::string ProductNotFound = "\033[1;31mproduct not found\033[0m\n";
    const std::string ExitList = "\nPress 0 to exit\n";
    const std::string EditProductSuccess = "\033[1;32medit product success\033[0m\n";
    const std::string EditProduct = "please input product name to edit: ";
    const std::string EditProductDesc = "please input product description(- for no change): ";
    const std::string EditProductPrice = "please input product price(-1 for no change): ";
    const std::string EditProductInventory = "please input product inventory(-1 for no change): ";
    const std::string EditProductCategory = "please input product category(- for no change): ";
    const std::string DeleteProduct = "please input product name to delete: ";
    const std::string DeleteProductSuccess = "\033[1;32mdelete product success\033[0m\n";
    const std::string SearchProduct = "please input product name to search: ";
    const std::string CartExist = "\033[1;31 is already in cart\033[0m\nAdd it anyway?(y/n): ";
    const std::string CartNotExist = "\033[1;31mProduct not exist in cart\033[0m\n";
    const std::string ProductNotExist = "\033[1;31mProduct not exist\033[0m\n";
    const std::string CartHead = "Name           category  desc                price     quantity      \n";
    const std::string CartTail = "---------------------------------------------------------------------\n";
    const std::string SumQua = "Total quantity: ";
    const std::string SumPrice = "Total price: ";
    const std::string AddCartSuccess = "\033[1;32madd cart success\033[0m\n";
    const std::string InputCartName = "please input product name: ";
    const std::string InputCartQua = "please input product quantity: ";
    const std::string EditCartSuccess = "\033[1;32medit cart success\033[0m\n";
    const std::string EditCart = "please input product name to edit: ";
    const std::string EditCartQua = "please input product quantity(-1 for no change): ";
    const std::string DeleteCart = "please input product name to delete: (split by ,)";
    const std::string DeleteCartSuccess = "\033[1;32mdelete cart success\033[0m\n";
    const std::string WantToDelete = "want to delete this product?(y/n): ";
    const std::string NeedLogin = "\033[1;31mplease login first\033[0m\n";

    // Order specific strings
    const std::string OrderLabelId = "Order ID: ";
    const std::string OrderLabelTime = "Time: ";
    const std::string OrderLabelStatus = "Status: ";
    const std::string OrderStatusPending = "Pending";
    const std::string OrderStatusShipped = "Shipped";
    const std::string OrderStatusDelivered = "Delivered";
    const std::string OrderLabelItems = "Items: ";
    const std::string OrderItemName = "\t- Name: ";
    const std::string OrderItemQuantity = ", Quantity: ";
    const std::string OrderLabelTotal = "Total: ";
    const std::string OrderLabelAddress = "Address: ";

    const std::string OrderNotFound = "\033[1;31mOrder not found\033[0m\n";
    const std::string ProductNotEnough = "\033[1;31mproduct not enough\033[0m\n";
    const std::string DeleteOrderSuccess = "\033[1;32mdelete order success\033[0m\n";
    const std::string DeleteOrderFail = "\033[1;31myou can only delete delivered order\033[0m\n";
    const std::string EditAddressSuccess = "\033[1;32medit address success\033[0m\n";
    const std::string EditAddressFail = "\033[1;31myou can only edit pending order\033[0m\n";
    const std::string OrderNotExist = "\033[1;31mOrder not exist\033[0m\n";
    const std::string CancelOrderSuccess = "\033[1;32mcancel order success\033[0m\n";
    const std::string CancelOrderFail = "\033[1;31myou can only cancel pending order\033[0m\n";
    const std::string OrderHead = "your order\n---------------------------------------------------------\n";
    const std::string Hr = "---------------------------------------------------------\n";
    const std::string InputOrderName = "please input product name: ";
    const std::string InputOrderQua = "please input product quantity: ";
    const std::string InputOrderAddress = "please input address: ";
    const std::string OrderSuccess = "\033[1;32morder success\033[0m\n";
    const std::string OrderFail = "\033[1;31morder fail\033[0m\n";
    const std::string ChangeAddressId = "please input order id to change: (you can only change pending order)";
    const std::string ChangeAddress = "please input address to change (you can only change pending order): ";
    const std::string DeleteOrder = "please input order id to delete (you can only delete delivered order): ";
    const std::string CancelOrder = "please input order id to cancel (you can only cancel pending order): ";
    const std::string CreateOrderFromCart = "do you want to buy items in cart?(y/n): ";
    const std::string ChangeOrderStatus = "please input order id to change status: ";
    const std::string ChangeOrderStatusInputStatus =
        "please input order status (pending, shipped, delivered, cancelled): ";
    const std::string ChangeOrderStatusSuccess = "\033[1;32mchange order status success\033[0m\n";
    const std::string AddOrderSuccess = "\033[1;32madd order success\033[0m\n";
    const std::string EmptyCart = "your cart is empty\n";
    const std::string SubmitOrderId = "Your order id is: ";
    const std::string AddThresholdCouponMinimum = "please input the minimum threshold: ";
    const std::string AddThresholdCouponDiscount = "please input the discount: ";
    const std::string AddThresholdCouponSuccess = "\033[1;32madd threshold coupon success\033[0m\n";
    const std::string AddThresholdCouponFail = "\033[1;31madd threshold coupon fail\033[0m\n";
    const std::string AddPercentCouponSuccess = "\033[1;32madd percent coupon success\033[0m\n";
    const std::string AddPercentCouponFail = "\033[1;31madd percent coupon fail\033[0m\n";
    const std::string AddPercentCouponName = "please input the name: ";
    const std::string AddPercentCouponCategory = "please input the category: ";
    const std::string AddPercentRatio = "please input the ratio: ";
    const std::string AddPercentDuration = "please set the offer duration (days): ";
}


inline Log logger;

#endif //LOG_H

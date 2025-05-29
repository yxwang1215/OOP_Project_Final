project(ooooop)
cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 26)
add_executable(new_target main.cpp
        cart.cpp
        log.cpp
        order.cpp
        product.cpp
        productList.cpp
        status.cpp
        user.cpp
        orderList.cpp
        coupon.cpp)

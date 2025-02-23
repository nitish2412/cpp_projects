#ifndef ORDER_HPP
#define ORDER_HPP

#include <string>

class Order {
private:
    std::string orderId;
    std::string instrument;
    double price;
    double quantity;
    std::string side; // "buy" or "sell"

public:
    Order(const std::string& id, const std::string& instrument, double price, double quantity, const std::string& side);

    std::string getOrderId() const;
    std::string getInstrument() const;
    double getPrice() const;
    double getQuantity() const;
    std::string getSide() const;

    void setPrice(double newPrice);
    void setQuantity(double newQuantity);
};

#endif // ORDER_HPP

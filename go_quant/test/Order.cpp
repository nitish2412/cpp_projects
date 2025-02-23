#include "../hpp/Order.hpp"

// Constructor
Order::Order(const std::string& id, const std::string& instrument, double price, double quantity, const std::string& side)
    : orderId(id), instrument(instrument), price(price), quantity(quantity), side(side) {}

// Getter methods
std::string Order::getOrderId() const { return orderId; }
std::string Order::getInstrument() const { return instrument; }
double Order::getPrice() const { return price; }
double Order::getQuantity() const { return quantity; }
std::string Order::getSide() const { return side; }

// Setter methods
void Order::setPrice(double newPrice) { price = newPrice; }
void Order::setQuantity(double newQuantity) { quantity = newQuantity; }

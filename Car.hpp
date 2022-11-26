#ifndef CAR_H
#define CAR_H

#include <vector>
#include <span>
#include <utility>
#include <cstdint>
#include <optional>

class Price {
  public:
    Price(uint64_t thousandths_of_a_dollar);
    Price(float dollars);
    uint64_t amount_in_thousandths_of_a_dollar() const;
    float amount_in_dollars() const;
  private:
    uint64_t thousandths_of_a_dollar;
};

Price operator+(Price rhs, Price lhs);

class Point2D {
  public:
    Point2D(double x, double y); 
    void set(double new_x,double new_y);
    void get(double *out_x, double *out_y) const;
    double distance(const Point2D& other) const;
  private:
    double x;
    double y;
};


class GasStation {
  public:
    GasStation(Price price_per_gallon, Point2D location);
    const Price price_per_gallon; 
    const Point2D location;
};

class Car {
  public: 
    Car(double tank_size, double miles_per_gallon);
    bool move_to(const Point2D& destination);
    std::optional<Price> fill_tank_at(GasStation station);
    bool can_move_to(const Point2D& destination) const;
    bool add_gas(double gallons);
    Point2D get_pos() const; 
    double get_gas() const; 
    double get_tank_size() const;
    double get_miles_per_gallon() const;
  private:
    Point2D pos;
    double gas;
    const double tank_size;
    const double miles_per_gallon;
};


std::vector<Car> meet_at_without_refueling(const std::span<Car> cars, Point2D destination);

std::vector< std::pair<Car, Price> > meet_at_with_refueling(
  const std::span<Car> cars, 
  const std::span<GasStation> gas_stations,
  Point2D destination
);

#endif

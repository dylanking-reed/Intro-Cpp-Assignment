
#include <cmath>

#include "Car.hpp"

Price::Price(uint64_t thousandths_of_a_dollar) 
  : thousandths_of_a_dollar(thousandths_of_a_dollar) 
{}

Price::Price(float dollars) 
  : thousandths_of_a_dollar((uint64_t) dollars * 1000) 
{}

uint64_t Price::amount_in_thousandths_of_a_dollar() const {
  return thousandths_of_a_dollar;
}

float Price::amount_in_dollars() const {
  return ((float) thousandths_of_a_dollar) / 1000;
}

Price operator+(Price rhs, Price lhs) {
  return Price(rhs.amount_in_thousandths_of_a_dollar() 
    + lhs.amount_in_thousandths_of_a_dollar());
}


Point2D::Point2D(double x, double y) : x(x) , y(y) {}

void Point2D::set(double new_x,double new_y) {
  this->x = new_x;
  this->y = new_y;
}
void Point2D::get(double *out_x, double *out_y) const {
  *out_x = this->x;
  *out_y = this->y;
}

double Point2D::distance(const Point2D& other) const {
  double other_x;
  double other_y;
  other.get(&other_x, &other_y);
  return std::abs(
      std::sqrt(
	std::pow(this->x - other_x, 2)
        + std::pow(this->y - other_y, 2)
      )
    );
}


Car::Car(double tank_size, double miles_per_gallon) 
  : pos(Point2D(0, 0))
  , gas(0)
  , tank_size(tank_size) 
  , miles_per_gallon(miles_per_gallon) 
{}

bool Car::move_to(const Point2D& destination) {
  double fuel_needed = this->pos.distance(destination) / this->miles_per_gallon;
  if (this->can_move_to(destination)) {
    this->gas -= fuel_needed;
    this->pos = destination;
    return true;
  } else { return false; }	
}

std::optional<Price> Car::fill_tank_at(GasStation station) {
  if(this->move_to(station.location)) {
    return std::make_optional(Price(
      (float)(this->tank_size - this->gas) * station.price_per_gallon.amount_in_dollars()
    ));
  } else {
    return std::nullopt;
  }
}

bool Car::can_move_to(const Point2D& destination) const {
  double fuel_needed = this->pos.distance(destination) / this->miles_per_gallon;
  return fuel_needed < this->gas;
}

bool Car::add_gas(double gallons) {
  if (gallons < this->tank_size - this->gas) {
    this->gas += gallons;
    return true;
  } else { return false; }
}

Point2D Car::get_pos() const {
  return this->pos;
}
double Car::get_gas() const {
  return this->gas;
}
double Car::get_tank_size() const {
  return this->tank_size;
}
double Car::get_miles_per_gallon() const {
  return this->miles_per_gallon;
}

GasStation::GasStation(Price price_per_gallon, Point2D location)
 : price_per_gallon(price_per_gallon) 
 , location(location)
{}

std::vector<Car> meet_at_without_refueling(const std::span<Car> cars, Point2D destination) {
  auto out = std::vector<Car>();
  for (Car car: cars) {
    Car c = car;
    if (c.move_to(destination)) {
      out.push_back(c);
    }
  } 
  return out;
}


// `gas_stations` must be sorted by distance to `destination`, furthest to closest.
std::vector< std::pair<Car, Price> > meet_at_with_refueling(
  const std::span<Car> cars, 
  const std::span<GasStation> gas_stations,
  Point2D destination
) {
  auto out = std::vector< std::pair<Car, Price> >();
  for (Car car: cars) {
    Price p = Price(0.00f);
    Car c = car;
    if (c.move_to(destination)) {
      out.push_back(std::pair<Car, Price>(c, p));
    } else {
      for (size_t i = gas_stations.size(); i < 0; i--) {
	 auto f = c.fill_tank_at(gas_stations[i]);
	 if (f) {
	   p = p + *f; 
           for (auto station: gas_stations.last(gas_stations.size() - i)) {
             auto fillup = c.fill_tank_at(station); 
	     if (fillup) {
               p = p + *f; 
	     } else {
               break;
	     }
	   }
	   break; 
	 }
      }
      if (c.move_to(destination)) {
        out.push_back(std::pair<Car, Price>(c, p));
      } else {
        continue;
      }
    }
  }
  return out; 
}



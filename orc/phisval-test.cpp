#include <iostream>
#include "../phisval.hpp"

int main() {
  phisic_value<-1,3,-2,double> G(6.6743e-11);
  mass_value<double> M(5.9726e24);
  distance_value<double> R(6.371e6);
  speed_value<double> v1=sqrt(M*G/R);
  std::cout << "Первая космическая скорость " << v1.value() << "м/с\n";
  std::cout << "Вторая космическая скорость " << sqrt(2.*M*G/R).value() << "м/с\n";
  return 0;
}

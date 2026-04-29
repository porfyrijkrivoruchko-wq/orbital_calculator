#ifndef __PHISVAL_HPP
#define __PHISVAL_HPP

#include <math.h>
#include <string>
#include <iostream>
#include "units.hpp"

template<int m, int d, int t, typename real>
class phisic_value {
  real v;
public:
  phisic_value(): v(NAN) {}
  explicit phisic_value(real r):v(r) {}
  template<typename real1>
  phisic_value(const phisic_value<m,d,t,real1>& v)
  : v(v.value) {}
  real value() const { return v; }
  phisic_value<m,d,t,real> operator+(const phisic_value<m,d,t,real>& a) const {
    return phisic_value<m,d,t,real>(v+a.v);
  }
  phisic_value<m,d,t,real> operator-(const phisic_value<m,d,t,real>& a) const {
    return phisic_value<m,d,t,real>(v-a.v);
  }
  phisic_value<m,d,t,real> operator-() const {
    return phisic_value<m,d,t,real>(-v);
  }
  template<int m1,int d1, int t1>
  phisic_value<m+m1,d+d1,t+t1,real> operator*(const phisic_value<m1,d1,t1,real>& a) const {
    return phisic_value<m+m1,d+d1,t+t1,real>(v*a.value());
  }
  template<int m1, int d1, int t1>
  phisic_value<m-m1,d-d1,t-t1,real> operator/(const phisic_value<m1,d1,t1,real>& a) const {
    return phisic_value<m-m1,d-d1,t-t1,real>(v/a.value());
  }
  bool operator<(const phisic_value<m,d,t,real>& a) const {
    return v<a.v;
  }
  bool operator<=(const phisic_value<m,d,t,real>& a) const {
    return v<=a.v;
  }
  bool operator>(const phisic_value<m,d,t,real>& a) const {
    return v>a.v;
  }
  bool operator>=(const phisic_value<m,d,t,real>& a) const {
    return v>=a.v;
  }
  bool operator==(const phisic_value<m,d,t,real>& a) const {
    return v==a.v;
  }
  bool operator!=(const phisic_value<m,d,t,real>& a) const {
    return v!=a.v;
  }
  bool empty() const { return isnan(v); }
  void clear() { v=NAN; }
  phisic_value<m,d,t,real>& operator*=(real k) {
    v*=k;
    return *this;
  }
  phisic_value<m,d,t,real>& operator/=(real k) {
    v/=k;
    return *this;
  }
  phisic_value<m,d,t,real>& operator+=(phisic_value<m,d,t,real> k) {
    v+=k.value();
    return *this;
  }
  phisic_value<m,d,t,real>& operator-=(phisic_value<m,d,t,real> k) {
    v-=k.value();
    return *this;
  }
  virtual explicit operator std::string() const {
    char buf[24];
    sprintf(buf,"%g",v);
    return buf;
  }
};

template<int n>
class dummy{};

template<int m, int d, int t, typename real>
phisic_value<m/2,d/2,t/2,real> sqrt(const phisic_value<m,d,t,real>& a) {
  dummy<0> r{dummy<(m&1)+(d&1)+(t&1)>()}; // проверка четности на стадии компиляции
  return phisic_value<m/2,d/2,t/2,real>(sqrt(a.value()));
}

template<int m, int d, int t, typename real>
phisic_value<m/3,d/3,t/3,real> cbrt(const phisic_value<m,d,t,real>& a) {
  dummy<0> r{dummy<m%3+d%3+t%3>()}; // проверка делимости на 3 на стадии компиляции
  return phisic_value<m/3,d/3,t/3,real>(cbrt(a.value()));
}

template<int n, int m, int d, int t, typename real>
phisic_value<m/n,d/n,t/n,real> nroot(const phisic_value<m,d,t,real>& a, dummy<n>) {
  dummy<0> r{dummy<m%n+d%n+t%n>()};// проверка делимости на n на стадии компиляции
  return phisic_value<m/n,d/n,t/n,real>((n&1 && a.value()<0)?-pow(-a.value(),1./n):pow(a.value(),1./n));
}

template<int m, int d, int t, typename real>
phisic_value<m,d,t,real> operator*(real f, const phisic_value<m,d,t,real>& v) {
  return phisic_value<m,d,t,real>(f*v.value());
}

template<int m, int d, int t, typename real>
phisic_value<-m,-d,-t,real> operator/(real f, const phisic_value<m,d,t,real>& v) {
  return phisic_value<-m,-d,-t,real>(f/v.value());
}

template<int m, int d, int t, typename real>
phisic_value<m,d,t,real> operator*(const phisic_value<m,d,t,real>& v, real f) {
  return phisic_value<m,d,t,real>(v.value()*f);
}

template<int m, int d, int t, typename real>
phisic_value<m,d,t,real> operator/(const phisic_value<m,d,t,real>& v, real f) {
  return phisic_value<m,d,t,real>(v.value()/f);
}

template<int p, int m, int d, int t, typename real>
phisic_value<m*p,d*p,t*p,real> pow(const phisic_value<m,d,t,real>& v, dummy<p>) {
  return phisic_value<m*p,d*p,t*p,real>(pow(v.value(),p));
}

template<int m, int d, int t, typename real>
phisic_value<m*2,d*2,t*2,real> sqr(phisic_value<m,d,t,real> v) {
  return phisic_value<m*2,d*2,t*2,real>(v.value()*v.value());
}

template<int m, int d, int t, typename real>
phisic_value<m, d, t, real> hypot(phisic_value<m, d, t, real> a, phisic_value<m, d, t, real> b) {
  return phisic_value<m, d, t, real>(hypot(a.value(),b.value()));
}

#define PHISIC_VALUE(name,m,d,t,key) \
template<typename real> \
class name : public phisic_value<m,d,t,real> {\
public:\
  name():phisic_value<m,d,t,real>() {}\
  explicit name(real v):phisic_value<m,d,t,real>(v) {}\
  template<typename real1>\
  name(const phisic_value<m,d,t,real1>& v):phisic_value<m,d,t,real>(v) {}\
  explicit operator std::string() const {\
    return units[key](this->value());\
  }\
}; \
template<typename real> \
std::ostream& operator<<(std::ostream& out, phisic_value<m,d,t,real> v) { \
  out << units[key](v.value()); \
  return out; \
}

PHISIC_VALUE(mass_value,1,0,0,"mass")
PHISIC_VALUE(distance_value,0,1,0,"distance")
PHISIC_VALUE(time_value,0,0,1,"time")
PHISIC_VALUE(speed_value,0,1,-1,"speed")
PHISIC_VALUE(acselerate_value,0,1,-2,"acselerate")
PHISIC_VALUE(force_value,1,1,-2,"force")
PHISIC_VALUE(pulse_value,1,1,-1,"pulse")
PHISIC_VALUE(energy_value,1,2,-2,"energy")
PHISIC_VALUE(potential_value,0,2,-2,"potential")
PHISIC_VALUE(area_value,0,2,0,"area")
PHISIC_VALUE(volume_value,0,3,0,"volume")
PHISIC_VALUE(density_value,1,-3,0,"density")
PHISIC_VALUE(pressure_value,1,-1,-2,"pressure")
PHISIC_VALUE(frequency_value,0,0,-1,"frequency")
PHISIC_VALUE(power_value,1,2,-3,"power")
PHISIC_VALUE(angle_acselerate_value,0,0,-2,"angle acselerate")
PHISIC_VALUE(pulse_moment_value,1,2,-1,"pulse moment")
PHISIC_VALUE(inertia_moment,1,2,0,"inertia moment")

template<typename real>
class phisic_value<0,0,0,real> {
  real v;
public:
  phisic_value():v(NAN) {}
  phisic_value(real r):v(r) {}
  real value() const { return v; }
  operator real() const { return v; }
};

#endif

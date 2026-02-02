#ifndef __PHISVAL_HPP
#define __PHISVAL_HPP

#include <math.h>

template<int m, int d, int t, typename real>
class phisic_value {
  real v;
public:
  phisic_value() {}
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

template<typename real>
class mass_value : public phisic_value<1,0,0,real>
{
public:
  mass_value():phisic_value<1,0,0,real>(){}
  explicit mass_value(real v):phisic_value<1,0,0,real>(v) {}
  template<typename real1>
  mass_value(const phisic_value<1,0,0,real1>& v):phisic_value<1,0,0,real>(v) {}
};

template<typename real>
class distance_value : public phisic_value<0,1,0,real>
{
public:
  distance_value():phisic_value<0,1,0,real>(){}
  explicit distance_value(real v):phisic_value<0,1,0,real>(v) {}
  template<typename real1>
  distance_value(const phisic_value<0,1,0,real1>& v):phisic_value<0,1,0,real>(v) {}
};

template<typename real>
class time_value : public phisic_value<0,0,1,real>
{
public:
  time_value():phisic_value<0,0,1,real>(){}
  explicit time_value(real v):phisic_value<0,0,1,real>(v) {}
  template<typename real1>
  time_value(const phisic_value<0,0,1,real1>& v):phisic_value<0,0,1,real>(v) {}
};

template<typename real>
class speed_value : public phisic_value<0,1,-1,real>
{
public:
  speed_value():phisic_value<0,1,-1,real>(){}
  explicit speed_value(real v):phisic_value<0,1,-1,real>(v) {}
  template<typename real1>
  speed_value(const phisic_value<0,1,-1,real1>& v):phisic_value<0,1,-1,real>(v) {}
};

template<typename real>
class acselerate_value : public phisic_value<0,1,-2,real>
{
public:
  acselerate_value():phisic_value<0,1,-2,real>(){}
  explicit acselerate_value(real v):phisic_value<0,1,-2,real>(v) {}
  template<typename real1>
  acselerate_value(const phisic_value<0,1,-2,real1>& v):phisic_value<0,1,-2,real>(v) {}
};

template<typename real>
class force_value : public phisic_value<1,1,-2,real>
{
public:
  force_value():phisic_value<1,1,-2,real>(){}
  explicit force_value(real v):phisic_value<1,1,-2,real>(v) {}
  template<typename real1>
  force_value(const phisic_value<1,1,-2,real1>& v):phisic_value<1,1,-2,real>(v) {}
};

template<typename real>
class pulse_value : public phisic_value<1,1,-1,real>
{
public:
  pulse_value():phisic_value<1,1,-1,real>(){}
  explicit pulse_value(real v):phisic_value<1,1,-1,real>(v) {}
  template<typename real1>
  pulse_value(const phisic_value<1,1,-1,real1>& v):phisic_value<1,1,-1,real>(v) {}
};

template<typename real>
class energy_value : public phisic_value<1,2,-2,real>
{
public:
  energy_value():phisic_value<1,2,-2,real>(){}
  explicit energy_value(real v):phisic_value<1,2,-2,real>(v) {}
  template<typename real1>
  energy_value(const phisic_value<1,2,-2,real1>& v):phisic_value<1,2,-2,real>(v) {}
};

template<typename real>
class potential_value : public phisic_value<0,2,-2,real>
{
public:
  potential_value():phisic_value<0,2,-2,real>(){}
  explicit potential_value(real v):phisic_value<0,2,-2,real>(v) {}
  template<typename real1>
  potential_value(const phisic_value<0,2,-2,real1>& v):phisic_value<0,2,-2,real>(v) {}
};

template<typename real>
class square_value : public phisic_value<0,2,0,real>
{
public:
  square_value():phisic_value<0,2,0,real>(){}
  explicit square_value(real v):phisic_value<0,2,0,real>(v) {}
  template<typename real1>
  square_value(const phisic_value<0,2,0,real1>& v):phisic_value<0,2,0,real>(v) {}
};

template<typename real>
class volume_value : public phisic_value<0,3,0,real>
{
public:
  volume_value():phisic_value<0,3,0,real>(){}
  explicit volume_value(real v):phisic_value<0,3,0,real>(v) {}
  template<typename real1>
  volume_value(const phisic_value<0,3,0,real1>& v):phisic_value<0,3,0,real>(v) {}
};

template<typename real>
class phisic_value<0,0,0,real> {
  real v;
public:
  phisic_value();
  phisic_value(real r):v(r) {}
  real value() const { return v; }
  operator real() const { return v; }
};

#endif

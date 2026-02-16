#ifndef SKYMECH_HPP
#define SKYMECH_HPP

#include <math.h>
#include <string>
#include <map>
#include <algorithm>
#include "../phisval.hpp"

typedef double Real;

const phisic_value<-1,3,-2,double> Gamma(6.6743e-11);
// #define Gamma (Real(6.6743e-11))

struct Skybody {
  std::string enname,runame;
  distance_value<Real> radius;
  mass_value<Real> mass;
  time_value<Real> period;
};

class Unit {
  std::map<std::string, Real> vlst;
  std::string dflt;
public:
  Unit() {}
  void add(std::string n, Real v) {
    vlst.insert(std::pair<std::string,Real>{n,v});
  }
  Real operator()(std::string) const;
  Real operator[](std::string n) const {
    auto p=vlst.find(n);
    if(p!=vlst.end()) return (*p).second;
    else return NAN;
  }
  typedef std::map<std::string, Real>::const_iterator iterator;
  iterator begin() const { return vlst.begin(); }
  iterator end() const { return vlst.end(); }
  std::string operator()(Real) const;
  void setdflt(std::string u) {
    if(vlst.count(u))
      dflt=u;
  }
  const std::string& getdflt() const {
    return dflt;
  }
};

enum Orbit_type { CIRCLE_ORBITE=0, ELLIPSE_ORBIT=1, PARABOLIC_ORBIT=2, HYPERBOLIC_ORBIT=3};

class CircleOrbit;
class EllipseOrbit;
class ParabolicOrbit;
class HyperbolicOrbit;

class Orbit {
protected:
  Skybody c;
  phisic_value<0,3,-2,Real> MG;
public:
  virtual Orbit_type type() const =0;
  Orbit(const Skybody& b) : c(b), MG(b.mass*Gamma) {}
  virtual ~Orbit() {}
  virtual operator CircleOrbit() const=0;
  virtual operator EllipseOrbit() const=0;
  virtual operator ParabolicOrbit() const=0;
  virtual operator HyperbolicOrbit() const=0;
  virtual bool legal() const=0;
  const Skybody& center() const { return c; }
  virtual Orbit* copy() const=0;
};

class CircleOrbit : public Orbit {
  distance_value<Real> r;
  speed_value<Real> s;
  time_value<Real> p;
public:
  CircleOrbit(const Skybody& b) 
  : Orbit(b) {}
  ~CircleOrbit() {}
  Orbit_type type() const { return CIRCLE_ORBITE; }
  void setradius(distance_value<Real>);
  void setspeed(speed_value<Real>);
  void setperiod(time_value<Real>);
  operator CircleOrbit() const { return *this; }
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const;
  bool legal() const;
  distance_value<Real> radius() const { return r; }
  speed_value<Real> speed() const { return s; }
  time_value<Real> period() const { return p; }
  Orbit* copy() const {
    return new CircleOrbit(*this);
  }
};

class EllipseOrbit : public Orbit {
  distance_value<Real> rp, ra, a, b;
  speed_value<Real> vp, va;
  time_value<Real> t;
  Real e;
  void calcrpra();
  void calcarp();
  void calcerp();
  void calcbrp();
  void calcrpvp();
  void calcrpva();
  void calcara();
  void calcbra();
  void calcera();
  void calcravp();
  void calcrava();
  void calcavp();
  void calcbvp();
  void calcevp();
  void calcvpva();
  void calcava();
  void calcbva();
  void calceva();
  void calcab();
  void calcae();
  void calcbe();
public:
  EllipseOrbit(const Skybody& b)
  : Orbit(b), e(NAN) {}
  ~EllipseOrbit() {}
  Orbit_type type() const { return ELLIPSE_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const { return *this; }
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const;
  bool legal() const;
  distance_value<Real> perigey() const { return rp; }
  distance_value<Real> apogey() const { return ra; }
  speed_value<Real> pspeed() const { return vp; }
  speed_value<Real> aspeed() const { return va; }
  time_value<Real> period() const { return t; }
  Real excentrisitet() const { return e; }
  distance_value<Real> bigsize() const { return a; }
  distance_value<Real> smallsize() const { return b; }
  void setperigey(distance_value<Real>);
  void setapogey(distance_value<Real>);
  void setpspeed(speed_value<Real>);
  void setaspeed(speed_value<Real>);
  void setperiod(time_value<Real>);
  void setexcentrisitet(Real);
  void setbigsize(distance_value<Real>);
  void setsmallsize(distance_value<Real>);
  Orbit* copy() const {
    return new EllipseOrbit(*this);
  }
};

class ParabolicOrbit : public Orbit {
  distance_value<Real> rp;
  speed_value<Real> vp;
public:
  ParabolicOrbit(const Skybody& b)
  : Orbit(b) {}
  ~ParabolicOrbit() {}
  Orbit_type type() const { return PARABOLIC_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const { return *this; }
  operator HyperbolicOrbit() const;
  bool legal() const;
  distance_value<Real> perigey() const { return rp; }
  speed_value<Real> pspeed() const { return vp; }
  void setperigey(distance_value<Real>);
  void setpspeed(speed_value<Real>);
  Orbit* copy() const {
    return new ParabolicOrbit(*this);
  }
};

class HyperbolicOrbit : public Orbit {
  distance_value<Real> rp, d;
  speed_value<Real> vp, vi;
  Real t,e;
  void calcrpvp();
  void calcrpvi();
  void calcerp();
  void calcdrp();
  void calcvpvi();
  void calcevp();
  void calcdvp();
  void calcevi();
  void calcde();
  void calcdvi();
public:
  HyperbolicOrbit(const Skybody& b)
  : Orbit(b), t(NAN), e(NAN) {}
  ~HyperbolicOrbit() {}
  Orbit_type type() const { return HYPERBOLIC_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const { return *this; }
  bool legal() const;
  distance_value<Real> perigey() const { return rp; }
  speed_value<Real> pspeed() const { return vp; }
  speed_value<Real> ispeed() const { return vi; }
  Real turn() const { return t; }
  Real excentrisitet() const { return e; }
  distance_value<Real> aiming() const { return d; }
  void setperigey(distance_value<Real>);
  void setpspeed(speed_value<Real>);
  void setispeed(speed_value<Real>);
  void setturn(Real);
  void setexcentrisitet(Real);
  void setaiming(distance_value<Real>);
  Orbit* copy() const {
    return new HyperbolicOrbit(*this);
  }
};

extern std::map<std::string, Skybody*> skybodies;
extern Unit massunit, sizeunit, timeunit, speedunit, angleunit;

template<int m, int d, int t, typename real>
phisic_value<m/2,d/2,t/2,real> cubicsolve(phisic_value<m,d,t,real> p, phisic_value<m+m/2,d+d/2,t+t/2,real> q) {
  dummy<0> r{dummy<m%2+d%2+t%2>()}; // контроль чётности
  auto D=p*p*p/27.+q*q/4.;
  if(D.value()>0) {
    auto R=sqrt(D);
    return cbrt(R-q/2.)-cbrt(R+q/2.);
  } else
    return phisic_value<m/2,d/2,t/2,real>();
}

class Point {
  const Skybody* center;
  Orbit* orb;
  distance_value<Real> dist, ro;
  Real phi;
  time_value<Real> t;
  speed_value<Real> sp, tsp, nsp;
  void crtorbit();
  void calc();
public:
  Point(const Skybody& body): center(&body), orb(NULL), phi(NAN) {}
  ~Point() { delete orb; }
  const Orbit* orbit() const { return orb; }
  distance_value<Real> distance() const { return dist; }
  distance_value<Real> aiming() const { return ro; }
  Real pitch() const { return phi; }
  time_value<Real> time() const { return t; }
  speed_value<Real> speed() const { return sp; }
  speed_value<Real> tspeed() const { return tsp; }
  speed_value<Real> nspeed() const { return nsp; }
  void setdistance(distance_value<Real>);
  void setaiming(distance_value<Real>);
  void setpitch(Real);
  void settime(time_value<Real>);
  void setspeed(speed_value<Real>);
  void settspeed(speed_value<Real>);
  void setnspeed(speed_value<Real>);
  void clear();
};

template<typename real, typename func_t>
real dihotomia(real a, real b, func_t f, real acc=0) {
  real fa=f(a), fb=f(b);
  if(fa==0) return a;
  if(fb==0) return b;
  if(b-a<=acc) return (a+b)/real(2);
  if(fa*fb>0) throw std::string("dihotomia: bad arguments");
  do{
    real c=(a+b)/real(2), fc=f(c);
    if(fc==0) return c;
    if(fa*fc<0) b=c; else a=c; 
  } while(b-a>acc);
  return (a+b)/real(2);
}

template<typename real, typename func_t>
real chordes(real a, real b, func_t f, real acc=0) {
  real fa=f(a), fb=f(b);
  if(fa==0) return a;
  if(fb==0) return b;
  if(b-a<=acc) return (a+b)/real(2);
  if(fa*fb>0) throw std::string("chordes: bad arguments");
  for(real last=a;;) {
    real c=(a*fb-b*fa)/(fb-fa), fc=f(c);
    if(fc==0) return c;
    if(fa*fc<0) b=c; else a=c; 
    if(std::abs(c-last)<=acc)
      return c;
    last=c;
  }
}

#endif

#ifndef SKYMECH_HPP
#define SKYMECH_HPP

#include <math.h>
#include <string>
#include <map>
#include <algorithm>
#include "phisval.hpp"

const phisic_value<-1,3,-2,double> Gamma(6.6743e-11);
// #define Gamma (Real(6.6743e-11))

struct Skybody {
  std::string enname,runame;
  distance_value<Real> radius;
  mass_value<Real> mass;
  time_value<Real> period;
  volume_value<Real> volume() const {
    return 4./3*M_PI*pow(radius,dummy<3>());
  }
  area_value<Real> surface() const {
    return 4*M_PI*sqr(radius);
  }
  density_value<Real> density() const {
    return mass/volume();
  }
  acselerate_value<Real> gravity(Real w) const {
    return mass*Gamma/sqr(radius)-radius/sqr(period/(2*M_PI))*cos(w);
  }
};

enum Orbit_type { UNNOWN_ORBIT, CIRCLE_ORBITE=1, ELLIPSE_ORBIT=2, PARABOLIC_ORBIT=3, HYPERBOLIC_ORBIT=4, LIFT_ORBIT};

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
  virtual CircleOrbit circle() const=0;
  virtual EllipseOrbit ellipse() const=0;
  virtual ParabolicOrbit parabola() const=0;
  virtual HyperbolicOrbit hyperbola() const=0;
  virtual bool legal() const=0;
  const Skybody& center() const { return c; }
  virtual Orbit* copy() const=0;
  virtual distance_value<Real> perigey() const=0;
  virtual speed_value<Real> pspeed() const=0;
};

class Point;

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
  CircleOrbit circle() const { return *this; }
  EllipseOrbit ellipse() const;
  ParabolicOrbit parabola() const;
  HyperbolicOrbit hyperbola() const;
  bool legal() const;
  distance_value<Real> radius() const { return r; }
  distance_value<Real> perigey() const { return r; }
  speed_value<Real> speed() const { return s; }
  speed_value<Real> pspeed() const { return s; }
  time_value<Real> period() const { return p; }
  Orbit* copy() const {
    return new CircleOrbit(*this);
  }
  Point operator()(time_value<Real>);
  Point operator()(Real arg);
  Point pdir(Real d);
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
  CircleOrbit circle() const;
  EllipseOrbit ellipse() const { return *this; }
  ParabolicOrbit parabola() const;
  HyperbolicOrbit hyperbola() const;
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
  Point operator()(time_value<Real>);
  Point operator()(Real arg);
  Point operator()(distance_value<Real>);
  Point operator()(speed_value<Real>);
  Point pdir(Real d);
};

class ParabolicOrbit : public Orbit {
  distance_value<Real> rp;
  speed_value<Real> vp;
public:
  ParabolicOrbit(const Skybody& b)
  : Orbit(b) {}
  ~ParabolicOrbit() {}
  Orbit_type type() const { return PARABOLIC_ORBIT; }
  CircleOrbit circle() const;
  EllipseOrbit ellipse() const;
  ParabolicOrbit parabola() const { return *this; }
  HyperbolicOrbit hyperbola() const;
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
  CircleOrbit circle() const;
  EllipseOrbit ellipse() const;
  ParabolicOrbit parabola() const;
  HyperbolicOrbit hyperbola() const { return *this; }
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
extern std::vector<Skybody> lstbodies;

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
  Real theta, phi, dir;
  time_value<Real> t;
  speed_value<Real> sp, tsp, nsp;
  void crtorbit();
  void calc();
public:
  Point(const Skybody& body): center(&body), orb(NULL), theta(NAN) {}
  Point(const Orbit* o): center(&o->center()), orb(o->copy()) {}
  ~Point() { delete orb; }
  const Orbit* orbit() const { return orb; }
  distance_value<Real> distance() const { return dist; }
  distance_value<Real> aiming() const { return ro; }
  Real pitch() const { return theta; }
  Real argument() const { return phi; }
  time_value<Real> time() const { return t; }
  speed_value<Real> speed() const { return sp; }
  speed_value<Real> tspeed() const { return tsp; }
  speed_value<Real> nspeed() const { return nsp; }
  void setdistance(distance_value<Real>);
  void setaiming(distance_value<Real>);
  void setpitch(Real);
  void setspeed(speed_value<Real>);
  void settspeed(speed_value<Real>);
  void setnspeed(speed_value<Real>);
  void clear();
  friend class CircleOrbit;
  friend class EllipseOrbit;
  friend class ParabolicOrbit;
  friend class HyperbolicOrbit;
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

inline Skybody operator*(Real k, const Skybody& b) {
  Skybody r(b);
  r.mass*=k;
  return r;
}

class double_system {
  const Skybody *first, *second;
  Skybody c1;
  EllipseOrbit orb;
  distance_value<Real> l[5];
public:
  double_system(const Skybody& b1, const Skybody& b2)
  : first(&b1), second(&b2), c1(sqr(b1.mass/(b1.mass+b2.mass))*b1),orb(c1) {}
  void setmaxdistance(distance_value<Real>);
  void setmindistance(distance_value<Real>);
  void setperiod(time_value<Real>);
  void setavgdistance(distance_value<Real>);
  void setexcentrisitet(Real);
  const Skybody& operator[](int i) const {
    if(i&1) return *second;
    else return *first;
  }
  distance_value<Real> maxdistance() const;
  distance_value<Real> mindistance() const;
  distance_value<Real> avgdistance() const;
  distance_value<Real> perigely(int=1) const;
  distance_value<Real> afely(int=1) const;
  time_value<Real> period() const;
  speed_value<Real> pspeed(int=1) const;
  speed_value<Real> aspeed(int=1) const;
  Real excentrisitet() const;
};

class OrbitalLift {
  const Skybody c;
  mass_value<Real> m1,m2,m3,mtr;
  distance_value<Real> r1,r2,l,x;
  time_value<Real> t;
  frequency_value<Real> w;
  density_value<Real> cd;
  pressure_value<Real> st;
  area_value<Real> crs;
  void calc();
public:
  OrbitalLift(const Skybody& b): c(b),x(distance_value<Real>(0)) {}
  void setmass(mass_value<Real>,int);
  void setheight(distance_value<Real>,int);
  void setradius(distance_value<Real>,int);
  void setlength(distance_value<Real>);
  void setposition(distance_value<Real>);
  void setdensity(density_value<Real>);
  void setstrength(pressure_value<Real>);
  void setsection(area_value<Real>);
  const Skybody& center() const { return c; }
  mass_value<Real> mass(int) const;
  distance_value<Real> height(int) const;
  distance_value<Real> radius(int) const;
  distance_value<Real> length() const;
  distance_value<Real> position() const;
  time_value<Real> period() const;
  frequency_value<Real> angspeed() const;
  speed_value<Real> speed(int) const;
  force_value<Real> tension(distance_value<Real>) const;
  pressure_value<Real> strength() const {
    return st;
  }
  area_value<Real> cross_section() const {
    return crs;
  }
  density_value<Real> cable_density() const {
    return cd;
  }
  acselerate_value<Real> gravity(distance_value<Real>) const;
};

class engine {
  force_value<Real> trf;
  speed_value<Real> spi;
  mass_value<Real> f, d, F;
public:
  typedef struct {
    mass_value<Real> consumption;
    time_value<Real> duration;
    speed_value<Real> imp;
  } pulse;
  engine() {}
  void setforce(force_value<Real> f) {
    trf=f;
  }
  void setspimp(speed_value<Real> s) {
    spi=s;
  }
  void setfuel(mass_value<Real> m) {
    f=m;
    if(!d.empty())
      F=f+d;
    else if(!F.empty())
      d=F-f;
  }
  void setdry(mass_value<Real> m) {
    d=m;
    if(!f.empty())
      F=d+f;
    else if(!F.empty())
      f=F-d;
  }
  void setfull(mass_value<Real> m) {
    F=m;
    if(!d.empty())
      f=F-d;
    else if(!f.empty())
      d=F-f;
  }
  void addfuel(mass_value<Real> m) {
    if(!f.empty())
      f+=m;
    else
      f=m;
    if(!d.empty())
      F=d+f;
    else if(!F.empty())
      d=F-f;
  }
  pulse operator()(speed_value<Real> s) const {
    mass_value<Real> m=F*exp(-s/spi);
    pulse p;
//    if(m>=d) {
      p.consumption=F-m;
      p.duration=p.consumption*spi/trf;
      p.imp=s;
//    }
    return p;
  }
  pulse operator()(time_value<Real> t) const {
    pulse p;
    p.duration=t;
    p.consumption=t*trf/spi;
    if(p.consumption<=f) {
      p.imp=spi*log(F/(F-p.consumption));
    }
    return p;
  }
  pulse operator()(mass_value<Real> m) const {
    pulse p;
    if(m<=f) {
      p.consumption=m;
      p.duration=m*spi/trf;
      p.imp=spi*log(F/(F-m));
    }
    return p;
  }
  pulse max() const {
    pulse p;
    p.consumption=f;
    p.duration=f*spi/trf;
    p.imp=spi*log(F/d);
    return p;
  }
  mass_value<Real> fuel() const {
    return f;
  }
  mass_value<Real> dry() const {
    return d;
  }
  mass_value<Real> full() const {
    return F;
  }
};

#endif

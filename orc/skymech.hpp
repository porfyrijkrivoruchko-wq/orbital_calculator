#ifndef SKYMECH_HPP
#define SKYMECH_HPP

#include <math.h>
#include <string>
#include <map>

typedef double Real;

#define Gamma (Real(6.6743e-11))

struct Skybody {
  std::string enname,runame;
  Real radius,mass,period;
};

class Unit {
  std::map<std::string, Real> vlst;
public:
  Unit();
  void add(std::string n, Real v) {
    vlst.insert(std::pair<std::string,Real>{n,v});
  }
  Real operator()(std::string) const;
  Real operator[](std::string n) const {
    auto p=vlst.find(n);
    if(p!=vlst.end()) return (*p).second;
    else return NAN;
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
  Real MG;
public:
  virtual Orbit_type type() const =0;
  Orbit(const Skybody& b) : c(b), MG(MG=b.mass*Gamma) {}
  virtual operator CircleOrbit() const=0;
  virtual operator EllipseOrbit() const=0;
  virtual operator ParabolicOrbit() const=0;
  virtual operator HyperbolicOrbit() const=0;
  virtual bool legal() const=0;
  const Skybody& center() const { return c; }
};

class CircleOrbit : public Orbit {
  Real r, s, p;
public:
  CircleOrbit(const Skybody& b) 
  : Orbit(b), r(NAN), s(NAN), p(NAN) {}
  Orbit_type type() const { return CIRCLE_ORBITE; }
  void setradius(Real);
  void setspeed(Real);
  void setperiod(Real);
  operator CircleOrbit() const { return *this; }
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const;
  bool legal() const;
  Real radius() const { return r; }
  Real speed() const { return s; }
  Real period() const { return p; }
};

class EllipseOrbit : public Orbit {
  Real rp, ra, vp, va, t, e, a, b;
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
  : Orbit(b), rp(NAN), ra(NAN), vp(NAN), va(NAN), t(NAN), e(NAN), a(NAN), b(NAN) {}
  Orbit_type type() const { return ELLIPSE_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const { return *this; }
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const;
  bool legal() const;
  Real perigey() const { return rp; }
  Real apogey() const { return ra; }
  Real pspeed() const { return vp; }
  Real aspeed() const { return va; }
  Real period() const { return t; }
  Real excentrisitet() const { return e; }
  Real bigsize() const { return a; }
  Real smallsize() const { return b; }
  void setperigey(Real);
  void setapogey(Real);
  void setpspeed(Real);
  void setaspeed(Real);
  void setperiod(Real);
  void setexcentrisitet(Real);
  void setbigsize(Real);
  void setsmallsize(Real);
};

class ParabolicOrbit : public Orbit {
  Real rp, vp;
public:
  ParabolicOrbit(const Skybody& b)
  : Orbit(b), rp(NAN), vp(NAN) {}
  Orbit_type type() const { return PARABOLIC_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const { return *this; }
  operator HyperbolicOrbit() const;
  bool legal() const;
  Real perigey() const { return rp; }
  Real pspeed() const { return vp; }
  void setperigey(Real);
  void setpspeed(Real);
};

class HyperbolicOrbit : public Orbit {
  Real rp, vp, vi, t, e, d;
public:
  HyperbolicOrbit(const Skybody& b)
  : Orbit(b), rp(NAN), vp(NAN), vi(NAN), t(NAN), e(NAN), d(NAN) {}
  Orbit_type type() const { return HYPERBOLIC_ORBIT; }
  operator CircleOrbit() const;
  operator EllipseOrbit() const;
  operator ParabolicOrbit() const;
  operator HyperbolicOrbit() const { return *this; }
  bool legal() const;
  Real perigey() const { return rp; }
  Real pspeed() const { return vp; }
  Real ispeed() const { return vi; }
  Real turn() const { return t; }
  Real excentrisitet() const { return e; }
  Real goal() const { return d; }
  void setperigey(Real);
  void setpspeed(Real);
  void setispeed(Real);
  void setturn(Real);
  void setexcentrisitet(Real);
  void setgoal(Real);
};

#endif

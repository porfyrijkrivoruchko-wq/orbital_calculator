#include <ctype.h>
#include <string.h>
#include <vector>
#include "skymech.hpp"

Real Unit::operator()(std::string v) const {
  char* tail;
  Real r=strtod(v.c_str(),&tail);
  while(isspace(*tail)) tail++;
  if(!*tail) return r*(*vlst.find(dflt)).second;
  auto i=vlst.lower_bound(tail);
  if(i!=vlst.end() && !strncmp(tail,(*i).first.c_str(),(*i).first.size()))
    return r*(*i).second;
  return r;
}

std::string Unit::operator()(Real v) const {
  char buf[32];
  sprintf(buf,"%g %s", v/(*vlst.find(dflt)).second, dflt.c_str());
  return buf;
}

void CircleOrbit::setradius(distance_value<Real> _r) {
  r=_r;
  s=sqrt(M_2_PI*MG/r);
  p=M_2_PI*r/s;
}

void CircleOrbit::setspeed(speed_value<Real> _s) {
  s=_s;
  r=MG/sqr(s);
  p=M_2_PI*MG/pow(s,dummy<3>());
}

void CircleOrbit::setperiod(time_value<Real> _p) {
  p=_p;
  s=cbrt(M_2_PI*MG/p);
  r=s*p/M_2_PI;
}

bool CircleOrbit::legal() const {
  if(!r.empty() && r<c.radius)
    return false;
  if(!s.empty() && s<=speed_value<Real>(0))
    return false;
  if(!p.empty() && p<=time_value<Real>(0))
    return false;
  return true;
}

CircleOrbit::operator EllipseOrbit() const {
  EllipseOrbit e(c);
  if(!r.empty()) {
    e.setperigey(r);
    e.setapogey(r);
  } else if(!s.empty()) {
    e.setpspeed(s);
    e.setaspeed(s);
  } else if(!p.empty()) {
    e.setperiod(p);
    e.setexcentrisitet(1.);
  }
  return e;
}

CircleOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit po(c);
  if(!r.empty())
    po.setperigey(r);
  else if(!s.empty())
    po.setpspeed(s);
  return po;
}

CircleOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!r.empty())
    h.setperigey(r);
  else if(!s.empty())
    h.setpspeed(s);
  return h;
}

EllipseOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!a.empty())
    co.setradius(a);
  else if(!rp.empty())
    co.setradius(rp);
  else if(!ra.empty())
    co.setradius(ra);
  else if(!b.empty())
    co.setradius(b);
  else if(!vp.empty())
    co.setspeed(vp);
  else if(!va.empty())
    co.setspeed(va);
  else if(!t.empty())
    co.setperiod(t);
  return co;
}

EllipseOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit p(c);
  if(!rp.empty())
    p.setperigey(rp);
  else if(!ra.empty())
    p.setperigey(ra);
  else if(!vp.empty())
    p.setpspeed(vp);
  else if(!va.empty())
    p.setpspeed(va);
  return p;
}

EllipseOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!rp.empty())
    h.setperigey(rp);
  else if(!ra.empty())
    h.setperigey(ra);
  else if(!vp.empty())
    h.setpspeed(vp);
  else if(!va.empty())
    h.setispeed(va);
  else if(!isnan(e))
    h.setexcentrisitet(e<1.?1./e:e);
  return h;
}

void EllipseOrbit::calcarp() {
  ra=2.*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbrp() {
  ra=sqr(b)/rp;
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpra() {
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(ra-rp)/(ra+rp);
  b=a*sqrt(1.-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcerp() {
  ra=rp*(1+e)/(1-e);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  b=a*sqrt(1-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpvp() {
  ra=rp*rp/(2.*MG/sqr(vp)-rp);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1-e*e);
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpva() {
  ra=sqrt(sqr(rp)/4.+2.*MG*rp/sqr(va))-rp/2.;
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1-e*e);
  vp=va*ra/rp;
}

void EllipseOrbit::calcara() {
  rp=2.*a-ra;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  t=M_2_PI*a*sqrt(a/MG);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbra() {
  rp=b*b/ra;
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcera() {
  rp=ra*(1+e)/(1-e);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrava() {
  rp=ra*ra/(2.*MG/(va*va)-ra);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  vp=va*ra/rp;
}

void EllipseOrbit::calcravp() {
  rp=sqrt(sqr(ra)/4.+2.*MG*ra/sqr(vp))-ra/2.;
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  va=vp*rp/ra;
}

void EllipseOrbit::calcab() {
  rp=a-sqrt(a*a-b*b);
  ra=2.*a-rp;
  e=(a-rp)/a;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcae() {
  rp=a*(1.-e);
  ra=a*(1.+e);
  b=a*sqrt(1.-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcava() {
  vp=MG/(va*a);
  rp=2.*MG/(vp*(vp+va));
  ra=2.*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcavp() {
  va=MG/(vp*a);
  rp=2.*MG/(vp*(vp+va));
  ra=2.*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcbe() {
  a=b/sqrt(1.-e*e);
  t=M_2_PI*a*sqrt(a/MG);
  rp=a-sqrt(a*a-b*b);
  ra=2.*a-rp;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbva() {
  ra=cubicsolve(b*b,-2.*MG*(b*b)/(va*va));
  rp=ra*ra/(2.*MG/(va*va)-ra);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=va*ra/rp;
}

void EllipseOrbit::calcbvp() {
  rp=cubicsolve(sqr(b),-2.*MG*sqr(b)/sqr(vp));
  ra=sqr(rp)/(2.*MG/sqr(vp)-rp);
  a=(rp+ra)/2.;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  va=vp*rp/ra;
}

void EllipseOrbit::calceva() {
  vp=va*(1.+e)/(1.-e);
  a=MG/(vp*va);
  t=M_2_PI*a*sqrt(a/MG);
  rp=2.*a*va/(vp+va);
  ra=2.*a-rp;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcevp() {
  va=vp*(1.-e)/(1.+e);
  a=MG/(vp*va);
  t=M_2_PI*a*sqrt(a/MG);
  rp=2.*a*va/(vp+va);
  ra=2.*a-rp;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcvpva() {
  a=MG/(vp*va);
  t=M_2_PI*a*sqrt(a/MG);
  e=(vp-va)/(vp+va);
  rp=2.*a*va/(vp+va);
  ra=2.*a-rp;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::setperigey(distance_value<Real> p) {
  rp=p;
  if(!ra.empty())
    calcrpra();
  else if(!a.empty())
    calcarp();
  else if(!isnan(e))
    calcerp();
  else if(!b.empty())
    calcbrp();
  else if(!vp.empty())
    calcrpvp();
  else if(!va.empty())
    calcrpva();
}

void EllipseOrbit::setapogey(distance_value<Real> r) {
  ra=r;
  if(!rp.empty())
    calcrpra();
  else if(!a.empty())
    calcara();
  else if(!b.empty())
    calcbra();
  else if(!isnan(e))
    calcera();
  else if(!vp.empty())
    calcravp();
  else if(!va.empty())
    calcrava();
}

void EllipseOrbit::setpspeed(speed_value<Real> s) {
  vp=s;
  if(!rp.empty())
    calcrpvp();
  else if(!ra.empty())
    calcravp();
  else if(!a.empty())
    calcavp();
  else if(!b.empty())
    calcbvp();
  else if(!isnan(e))
    calcevp();
  else if(!va.empty())
    calcvpva();
}

void EllipseOrbit::setaspeed(speed_value<Real> s) {
  va=s;
  if(!ra.empty())
    calcravp();
  else if(!rp.empty())
    calcrpva();
  else if(!a.empty())
    calcava();
  else if(!b.empty())
    calcbva();
  else if(!isnan(e))
    calceva();
  else if(!vp.empty())
    calcvpva();
}

void EllipseOrbit::setbigsize(distance_value<Real> s) {
  a=s;
  t=M_2_PI*a*sqrt(a/MG);
  if(!b.empty())
    calcab();
  else if(!rp.empty())
    calcarp();
  else if(!ra.empty())
    calcara();
  else if(!isnan(e))
    calcae();
  else if(!va.empty())
    calcava();
  else if(!vp.empty())
    calcavp();
}

void EllipseOrbit::setsmallsize(distance_value<Real> s) {
  a=s;
  if(!a.empty())
    calcab();
  else if(!isnan(e))
    calcbe();
  else if(!rp.empty())
    calcbrp();
  else if(!ra.empty())
    calcbrp();
  else if(!vp.empty())
    calcbvp();
  else if(!va.empty())
    calcbva();
}

void EllipseOrbit::setexcentrisitet(Real ex) {
  e=ex;
  if(!a.empty())
    calcae();
  else if(!b.empty())
    calcbe();
  else if(!rp.empty())
    calcerp();
  else if(!ra.empty())
    calcera();
  else if(!vp.empty())
    calcevp();
  else if(!va.empty())
    calceva();
}

void EllipseOrbit::setperiod(time_value<Real> p) {
  t=p;
  a=cbrt(MG*sqr(t/M_2_PI));
  if(!b.empty())
    calcab();
  else if(!rp.empty())
    calcarp();
  else if(!ra.empty())
    calcara();
  else if(!isnan(e))
    calcae();
  else if(!va.empty())
    calcava();
  else if(!vp.empty())
    calcavp();
}

bool EllipseOrbit::legal() const {
  if(!rp.empty() && rp<c.radius)
    return false;
  if(!rp.empty() && !ra.empty() && rp>ra)
    return false;
  if(!isnan(e) && (e<0 || e>=1.))
    return false;
  if(!a.empty() && a<c.radius)
    return false;
  if(!b.empty() && b<c.radius)
    return false;
  return true;
}

ParabolicOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!rp.empty())
    co.setradius(rp);
  return co;
}

ParabolicOrbit::operator EllipseOrbit() const {
  EllipseOrbit e(c);
  if(!rp.empty())
    e.setperigey(rp);
  return e;
}

ParabolicOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!rp.empty())
    h.setperigey(rp);
  if(!vp.empty())
    h.setpspeed(vp);
  return h;
}

void ParabolicOrbit::setperigey(distance_value<Real> p) {
  rp=p;
  vp=sqrt(2.*MG/rp);
}

void ParabolicOrbit::setpspeed(speed_value<Real> s) {
  vp=s;
  rp=2.*MG/(vp*vp);
}

bool ParabolicOrbit::legal() const {
  if(!rp.empty() && rp<c.radius)
    return false;
  return true;
}

void HyperbolicOrbit::calcde() {
  rp=d*sqrt((e-1.)/(e+1.));
  vp=sqrt(MG*(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcdrp() {
  e=(sqr(d)+sqr(rp))/(sqr(d)-sqr(rp));
  t=2.*asin(1./e);
  vp=sqrt(MG*(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcdvi() {
  speed_value<Real> f=MG/(vi*d);
  vp=f+hypot(f,vi);
  rp=2.*MG/(sqr(vp)-sqr(vi));
  e=(sqr(vp)+sqr(vi))/(sqr(vp)-sqr(vi));
  t=2*asin(1./e);
}

void HyperbolicOrbit::calcdvp() {
  // Здесь нужно решать кубическое уравнение
  vi=cubicsolve(-sqr(vp),2.*MG*vp/d);
  if(vi.empty()) // Если более одного корня, оставляем значения неопределенными
    d.clear();
  else {
    rp=2.*MG/(sqr(vp)-sqr(vi));
    e=(sqr(vp)+sqr(vi))/(sqr(vp)-sqr(vi));
    t=2*asin(1./e);
  }
}

void HyperbolicOrbit::calcerp() {
  d=rp*sqrt((e+1.)/(e-1.));
  vp=sqrt(MG*(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcevi() {
  vp=vi*sqrt((e+1.)/(e-1.));
  rp=2.*MG/(vp*vp-vi*vi);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcevp() {
  vi=vp*sqrt((e-1.)/(e+1.));
  rp=2.*MG/(sqr(vp)-sqr(vi));
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcrpvi() {
  vp=sqrt(sqr(vi)+2.*MG/rp);
  e=(sqr(vp)+sqr(vi))/(sqr(vp)-sqr(vi));
  t=2*asin(1./e);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcrpvp() {
  vi=sqrt(sqr(vp)-2.*MG/rp);
  e=(sqr(vp)+sqr(vi))/(sqr(vp)-sqr(vi));
  t=2*asin(1./e);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcvpvi() {
  rp=2.*MG/(sqr(vp)-sqr(vi));
  e=(sqr(vp)+sqr(vi))/(sqr(vp)-sqr(vi));
  t=2*asin(1./e);
  d=rp*vp/vi;
}

HyperbolicOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!rp.empty())
    co.setradius(rp);
  else if(!vp.empty())
    co.setspeed(vp);
  else if(!vi.empty())
    co.setspeed(vi);
  return co;
}

HyperbolicOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit p(c);
  if(!rp.empty())
    p.setperigey(rp);
  else if(!vp.empty())
    p.setpspeed(vp);
  else if(!vi.empty())
    p.setpspeed(vi);
  return p;
}

HyperbolicOrbit::operator EllipseOrbit() const {
  EllipseOrbit o(c);
  if(!rp.empty())
    o.setperigey(rp);
  if(!isnan(e))
    o.setexcentrisitet(e>1.?1./e:e);
  else if(!vp.empty())
    o.setpspeed(vp);
  return o;
}

void HyperbolicOrbit::setperigey(distance_value<Real> r) {
  rp=r;
  if(!vp.empty())
    calcrpvp();
  else if(!vi.empty())
    calcrpvi();
  else if(!isnan(e))
    calcerp();
  else if(!d.empty())
    calcdrp();
}

void HyperbolicOrbit::setpspeed(speed_value<Real> s) {
  vp=s;
  if(!rp.empty())
    calcrpvp();
  else if(!vi.empty())
    calcvpvi();
  else if(!isnan(e))
    calcevp();
  else if(!d.empty())
    calcdvp();
}

void HyperbolicOrbit::setispeed(speed_value<Real> s) {
  vi=s;
  if(!rp.empty())
    calcrpvi();
  else if(!vp.empty())
    calcvpvi();
  else if(!isnan(e))
    calcevi();
  else if(!d.empty())
    calcdvi();
}

void HyperbolicOrbit::setturn(Real _t) {
  t=_t;
  e=1./sin(t/2.);
  if(!rp.empty())
    calcerp();
  else if(!vp.empty())
    calcevp();
  else if(!vi.empty())
    calcevi();
  else if(!d.empty())
    calcde();
}

void HyperbolicOrbit::setaiming(distance_value<Real> g) {
  d=g;
  if(!rp.empty())
    calcdrp();
  else if(!vp.empty())
    calcdvp();
  else if(!vi.empty())
    calcdvi();
  else if(!isnan(e))
    calcde();
}

void HyperbolicOrbit::setexcentrisitet(Real _e) {
  e=_e;
  t=2*asin(1./e);
  if(!rp.empty())
    calcerp();
  else if(!vp.empty())
    calcevp();
  else if(!vi.empty())
    calcevi();
  else if(!d.empty())
    calcde();
}

bool HyperbolicOrbit::legal() const {
  if(!rp.empty() && rp<c.radius)
    return false;
  if(!isnan(e) && e<1.)
    return false;
  if(!vp.empty() && vp.value()<=0)
    return false;
  if(!vi.empty() && vi.value()<0)
    return false;
  if(!isnan(t) && (t<=0 || t>M_PI))
    return false;
  return true;
}

void Point::setpitch(Real a) {
  phi=a;
  bool ssd=false;
  if(!sp.empty()) {
    nsp=sp*sin(phi);
    tsp=sp*cos(phi);
  } else if(!nsp.empty()) {
    sp=nsp/sin(phi);
    tsp=nsp/tan(phi);
    ssd=true;
  } else if(!tsp.empty()) {
    sp=tsp/cos(phi);
    nsp=tsp*tan(phi);
    ssd=true;
  }
  if(!dist.empty())
    ro=dist*std::abs(cos(phi));
  else if(!ro.empty()) {
    dist=ro/std::abs(cos(phi));
    ssd=true;
  }
  if(ssd && !sp.empty() && !dist.empty())
    crtorbit();
  calc();
}

void Point::setdistance(distance_value<Real> d) {
  dist=d;
  if(!isnan(phi))
    ro=dist*std::abs(cos(phi));
  else if(!ro.empty()) {
    phi=acos(ro/dist);
    if(!sp.empty()){
      nsp=sp*sin(phi);
      tsp=sp*cos(phi);
    } else if(!nsp.empty()) {
      sp=nsp/sin(phi);
      tsp=nsp/tan(phi);
    } else if(!tsp.empty()) {
      sp=tsp/cos(phi);
      nsp=tsp*tan(phi);
    }
  }
  if(!sp.empty())
    crtorbit();
  calc();
}

void Point::setaiming(distance_value<Real> g) {
  ro=g;
  bool ssd=false;
  if(!dist.empty()) {
    phi=acos(ro/dist);
    if(!sp.empty()){
      nsp=sp*sin(phi);
      tsp=sp*cos(phi);
    } else if(!nsp.empty()) {
      sp=nsp/sin(phi);
      tsp=nsp/tan(phi);
      ssd=true;
    } else if(!tsp.empty()) {
      sp=tsp/cos(phi);
      nsp=tsp*tan(phi);
      ssd=true;
    }
  } else if(!isnan(phi)) {
    dist=ro/cos(phi);
    ssd=true;
  }
  if(ssd && !dist.empty() && !sp.empty())
    crtorbit();
  calc();
}

void Point::setnspeed(speed_value<Real> s) {
  nsp=s;
  bool ssd=false;
  if(!sp.empty()) {
    tsp=sqrt(sqr(sp)-sqr(nsp));
    phi=asin(nsp/sp);
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty()) {
      dist=ro/std::abs(cos(phi));
      ssd=true;
    }
  } else if(!isnan(phi)) {
    sp=nsp/sin(phi);
    tsp=nsp/tan(phi);
    ssd=true;
  } else if(!tsp.empty()) {
    phi=atan2(nsp.value(),tsp.value());
    sp=hypot(nsp,tsp);
    ssd=true;
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty())
      dist=ro/std::abs(cos(phi));
  }
  if(ssd && !dist.empty() && !sp.empty())
    crtorbit();
  calc();
}

void Point::setspeed(speed_value<Real> s) {
  sp=s;
  if(!isnan(phi)) {
    nsp=sp*sin(phi);
    tsp=sp*cos(phi);
  } else if(!nsp.empty()) {
    tsp=sqrt(sqr(sp)-sqr(nsp));
    phi=asin(nsp/sp);
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty())
      dist=ro/std::abs(cos(phi));
  } else if(!tsp.empty()) {
    nsp=sqrt(sqr(sp)-sqr(tsp));
    phi=acos(tsp/sp);
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty())
      dist=ro/std::abs(cos(phi));
  }
  if(!dist.empty())
    crtorbit();
  calc();
}

void Point::settspeed(speed_value<Real> s) {
  tsp=s;
  bool ssd=false;
  if(!sp.empty()) {
    nsp=sqrt(sqr(sp)-sqr(tsp));
    phi=acos(tsp/sp);
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty()) {
      dist=ro/std::abs(cos(phi));
      ssd=true;
    }
  } else if(!nsp.empty()) {
    sp=hypot(tsp,nsp);
    phi=atan2(nsp.value(),tsp.value());
    ssd=true;
    if(!dist.empty())
      ro=dist*std::abs(cos(phi));
    else if(!ro.empty())
      dist=ro/std::abs(cos(phi));
  } else if(!isnan(phi)) {
    sp=tsp/cos(phi);
    nsp=tsp*tan(phi);
    ssd=true;
  }
  if(ssd && !dist.empty() && !sp.empty())
    crtorbit();
  calc();
}

void Point::calc() {
  if(orb && !dist.empty() && !sp.empty() && !tsp.empty()) {
    auto MG=center->mass*Gamma;
    auto sp2=2.*MG/dist;
    distance_value<Real> p=sqr(tsp*dist)/(MG+sqrt(sqr(MG)-sqr(tsp*dist)*(sp2-sqr(sp))));
    switch(orb->type()) {
      case ELLIPSE_ORBIT:
        static_cast<EllipseOrbit*>(orb)->setperigey(p);
        break;
      case PARABOLIC_ORBIT:
        static_cast<ParabolicOrbit*>(orb)->setperigey(p);
        break;
      case HYPERBOLIC_ORBIT:
        static_cast<HyperbolicOrbit*>(orb)->setperigey(p);
        break;
      case CIRCLE_ORBITE:
        break;
    }
  }
}

void Point::crtorbit() {
  if(orb) delete orb;
  auto MG=center->mass*Gamma;
  speed_value<Real> sp2=sqrt(2.*MG/dist);
  if(sp<sp2) {
    orb=new EllipseOrbit(*center);
    static_cast<EllipseOrbit*>(orb)->setbigsize(1./(2./dist-sqr(sp)/MG));
  }
  else if(sp==sp2) {
    orb=new ParabolicOrbit(*center);
  }
  else {
    orb=new HyperbolicOrbit(*center);
    static_cast<HyperbolicOrbit*>(orb)->setispeed(sqrt(sqr(sp)-sqr(sp2)));
  }
}

void Point::settime(time_value<Real> _t) {
  t=_t;
}

void Point::clear() {
  if(orb) delete orb;
  dist.clear(); ro.clear();
  phi=NAN;
  t.clear();
  sp.clear(); tsp.clear(); nsp.clear();
}

std::vector<Skybody> lstbodies;
std::map<std::string, Skybody*> skybodies;
Unit massunit, sizeunit, timeunit, speedunit, angleunit;

class Initial {
public:
  Initial();
} initial;

Initial::Initial() {
  char *path;
  {
    char *p=strrchr(program_invocation_name,'/');
    if(p) path=strndup(program_invocation_name,p-program_invocation_name+1);
    else path=strdup("");
  }
  size_t size=0;
  char* line=NULL;
  FILE* f;
  {
    char funit[strlen(path)+10];
    strcat(strcpy(funit,path),"units.txt");
    f=fopen(funit,"r");
  }
  if(!f) perror("Не могу найти файл units.txt");
  Unit* cu=NULL;
  while(getline(&line,&size,f)>0) {
    if(strcasestr(line,"[mass]")) {
      cu=&massunit;
      continue;
    }
    if(strcasestr(line,"[distance]")) {
      cu=&sizeunit;
      continue;
    }
    if(strcasestr(line,"[time]")) {
      cu=&timeunit;
      continue;
    }
    if(strcasestr(line,"[speed]")) {
      cu=&speedunit;
      continue;
    }
    if(strcasestr(line,"[angle]")) {
      cu=&angleunit;
      continue;
    }
    if(!cu) continue;
    char* p=strtok(line," \t\n");
    if(!p) continue;
    Real v=atof(p);
    p=strtok(NULL,"\t\n");
    if(!p) continue;
    bool dflt=false;
    if(p[strlen(p)-1]=='*') {
      p[strlen(p)-1]=0;
      dflt=true;
    }
    cu->add(p,v);
    if(dflt)
      cu->setdflt(p);
  }
  fclose(f);
  {
    char fbodies[strlen(path)+14];
    strcat(strcpy(fbodies,path),"skybodies.txt");
    f=fopen(fbodies,"r");
  }
  if(!f) perror("Не могу найти файл skybodies.txt");
  while(getline(&line,&size,f)>0) {
    Skybody b;
    char* p=strtok(line,",\n");
    if(!p) continue;
    b.enname=p;
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.runame=p;
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.mass=mass_value<Real>(massunit(p));
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.radius=distance_value<Real>(sizeunit(p));
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.period=time_value<Real>(timeunit(p));
    lstbodies.push_back(b);
  }
  fclose(f);
  for(auto i=lstbodies.begin(); i!=lstbodies.end(); ++i) {
    skybodies[(*i).enname]=&*i;
    skybodies[(*i).runame]=&*i;
  }
}

#include <ctype.h>
#include <string.h>
#include "skymech.hpp"

Real Unit::operator()(std::string v) const {
  char* tail;
  Real r=strtod(v.c_str(),&tail);
  while(isspace(*tail)) tail++;
  if(!*tail) return r;
  auto i=vlst.lower_bound(tail);
  if(!strncmp(tail,(*i).first.c_str(),(*i).first.size()))
    return r*(*i).second;
  return r;
}

void CircleOrbit::setradius(Real _r) {
  r=_r;
  s=sqrt(M_2_PI*MG/r);
  p=M_2_PI*r/s;
}

void CircleOrbit::setspeed(Real _s) {
  s=_s;
  r=MG/(s*s);
  p=M_2_PI*MG/(s*s*s);
}

void CircleOrbit::setperiod(Real _p) {
  p=_p;
  s=pow(M_2_PI*MG/p,1./3);
  r=s*p/(M_2_PI);
}

bool CircleOrbit::legal() const {
  if(!isnan(r) && r<c.radius)
    return false;
  if(!isnan(s) && s<=0)
    return false;
  if(!isnan(p) && p<=0)
    return false;
  return true;
}

CircleOrbit::operator EllipseOrbit() const {
  EllipseOrbit e(c);
  if(!isnan(r)) {
    e.setperigey(r);
    e.setapogey(r);
  } else if(!isnan(s)) {
    e.setpspeed(s);
    e.setaspeed(s);
  } else if(!isnan(p)) {
    e.setperiod(p);
    e.setexcentrisitet(1.);
  }
  return e;
}

CircleOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit po(c);
  if(!isnan(r))
    po.setperigey(r);
  else if(!isnan(s))
    po.setpspeed(s);
  return po;
}

CircleOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!isnan(r))
    h.setperigey(r);
  else if(!isnan(s))
    h.setpspeed(s);
  return h;
}

EllipseOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!isnan(a))
    co.setradius(a);
  else if(!isnan(rp))
    co.setradius(rp);
  else if(!isnan(ra))
    co.setradius(ra);
  else if(!isnan(b))
    co.setradius(b);
  else if(!isnan(vp))
    co.setspeed(vp);
  else if(!isnan(va))
    co.setradius(va);
  else if(!isnan(t))
    co.setperiod(t);
  return co;
}

EllipseOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit p(c);
  if(isnan(rp))
    p.setperigey(rp);
  else if(!isnan(ra))
    p.setperigey(ra);
  else if(!isnan(vp))
    p.setpspeed(vp);
  else if(!isnan(va))
    p.setpspeed(va);
  return p;
}

EllipseOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!isnan(rp))
    h.setperigey(rp);
  else if(!isnan(ra))
    h.setperigey(ra);
  else if(!isnan(vp))
    h.setpspeed(vp);
  else if(!isnan(va))
    h.setispeed(va);
  else if(!isnan(e))
    h.setexcentrisitet(e<1.?1./e:e);
  return h;
}

void EllipseOrbit::calcarp() {
  ra=2*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbrp() {
  ra=b*b/rp;
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpra() {
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(ra-rp)/(ra+rp);
  b=a*sqrt(1-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcerp() {
  ra=rp*(1+e)/(1-e);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  b=a*sqrt(1-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpvp() {
  ra=rp*rp/(2*MG/(vp*vp)-rp);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1-e*e);
  va=vp*rp/ra;
}

void EllipseOrbit::calcrpva() {
  ra=(rp+sqrt(rp*rp+8*MG*rp*va*va))/(2*va*va);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1-e*e);
  vp=va*ra/rp;
}

void EllipseOrbit::calcara() {
  rp=2*a-ra;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  t=M_2_PI*a*sqrt(a/MG);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbra() {
  rp=b*b/ra;
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcera() {
  rp=ra*(1+e)/(1-e);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcrava() {
  rp=ra*ra/(2*MG/(va*va)-ra);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  vp=va*ra/rp;
}

void EllipseOrbit::calcravp() {
  rp=(ra+sqrt(ra*ra+8*MG*ra*vp*vp))/(2*vp*vp);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
  va=vp*rp/ra;
}

void EllipseOrbit::calcab() {
  rp=a-sqrt(a*a-b*b);
  ra=2*a-rp;
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
  rp=2*MG/(vp*(vp+va));
  ra=2*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcavp() {
  va=MG/(vp*a);
  rp=2*MG/(vp*(vp+va));
  ra=2*a-rp;
  e=(a-rp)/a;
  b=a*sqrt(1.-e*e);
}

void EllipseOrbit::calcbe() {
  a=b/sqrt(1.-e*e);
  t=M_2_PI*a*sqrt(a/MG);
  rp=a-sqrt(a*a-b*b);
  ra=2*a-rp;
  vp=sqrt(MG*ra/(a*rp));
  va=vp*rp/ra;
}

void EllipseOrbit::calcbva() {
  // Здесь нужно решать кубическое уравнение
}

void EllipseOrbit::calcbvp() {
  // Здесь нужно решать кубическое уравнение
}

void EllipseOrbit::calceva() {
  vp=va*(1.+e)/(1.-e);
  a=MG/(vp+va);
  t=M_2_PI*a*sqrt(a/MG);
  rp=2*a*va/(vp+va);
  ra=2*a-rp;
  b=a*sqrt(1.-a*a);
}

void EllipseOrbit::calcevp() {
  va=vp*(1.-e)/(1.+e);
  a=MG/(vp+va);
  t=M_2_PI*a*sqrt(a/MG);
  rp=2*a*va/(vp+va);
  ra=2*a-rp;
  b=a*sqrt(1.-a*a);
}

void EllipseOrbit::calcvpva() {
  a=MG/(vp+va);
  t=M_2_PI*a*sqrt(a/MG);
  e=(vp-va)/(vp+va);
  rp=2*a*va/(vp+va);
  ra=2*a-rp;
  b=a*sqrt(1.-a*a);
}

void EllipseOrbit::setperigey(Real p) {
  rp=p;
  if(!isnan(ra))
    calcrpra();
  else if(!isnan(a))
    calcarp();
  else if(!isnan(e))
    calcerp();
  else if(!isnan(b))
    calcbrp();
  else if(!isnan(vp))
    calcrpvp();
  else if(!isnan(va))
    calcrpva();
}

void EllipseOrbit::setapogey(Real r) {
  ra=r;
  if(!isnan(rp))
    calcrpra();
  else if(!isnan(a))
    calcara();
  else if(!isnan(b))
    calcbra();
  else if(!isnan(e))
    calcera();
  else if(!isnan(vp))
    calcravp();
  else if(!isnan(va))
    calcrava();
}

void EllipseOrbit::setpspeed(Real s) {
  vp=s;
  if(!isnan(rp))
    calcrpvp();
  else if(!isnan(ra))
    calcravp();
  else if(!isnan(a))
    calcavp();
  else if(!isnan(b))
    calcbvp();
  else if(!isnan(e))
    calcevp();
  else if(!isnan(va))
    calcvpva();
}

void EllipseOrbit::setaspeed(Real s) {
  va=s;
  if(!isnan(ra))
    calcravp();
  else if(!isnan(rp))
    calcrpva();
  else if(!isnan(a))
    calcava();
  else if(!isnan(b))
    calcbva();
  else if(!isnan(e))
    calceva();
  else if(!isnan(vp))
    calcvpva();
}

void EllipseOrbit::setbigsize(Real s) {
  a=s;
  t=M_2_PI*a*sqrt(a/MG);
  if(!isnan(b))
    calcab();
  else if(!isnan(rp))
    calcarp();
  else if(!isnan(ra))
    calcara();
  else if(!isnan(e))
    calcae();
  else if(!isnan(va))
    calcava();
  else if(!isnan(vp))
    calcavp();
}

void EllipseOrbit::setsmallsize(Real s) {
  a=s;
  if(!isnan(a))
    calcab();
  else if(!isnan(e))
    calcbe();
  else if(!isnan(rp))
    calcbrp();
  else if(!isnan(ra))
    calcbrp();
  else if(!isnan(vp))
    calcbvp();
  else if(!isnan(va))
    calcbva();
}

void EllipseOrbit::setexcentrisitet(Real ex) {
  e=ex;
  if(!isnan(a))
    calcae();
  else if(!isnan(b))
    calcbe();
  else if(!isnan(rp))
    calcerp();
  else if(!isnan(ra))
    calcera();
  else if(!isnan(vp))
    calcevp();
  else if(!isnan(va))
    calceva();
}

void EllipseOrbit::setperiod(Real p) {
  t=p;
  a=pow(MG,1./3)*pow(t/M_2_PI,2./3);
  if(!isnan(b))
    calcab();
  else if(!isnan(rp))
    calcarp();
  else if(!isnan(ra))
    calcara();
  else if(!isnan(e))
    calcae();
  else if(!isnan(va))
    calcava();
  else if(!isnan(vp))
    calcavp();
}

bool EllipseOrbit::legal() const {
  if(!isnan(rp) && rp<c.radius)
    return false;
  if(!isnan(rp) && !isnan(ra) && rp>ra)
    return false;
  if(!isnan(e) && (e<0 || e>=1.))
    return false;
  if(!isnan(a) && a<c.radius)
    return false;
  if(!isnan(b) && b<c.radius)
    return false;
  return true;
}

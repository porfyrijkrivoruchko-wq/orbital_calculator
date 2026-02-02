#include <ctype.h>
#include <string.h>
#include <vector>
#include "skymech.hpp"

Real cubicsolve(Real p, Real q) {
  Real d=p*p*p/27+q*q/4;
  if(d>0) {
    d=sqrt(d);
    return cbrt(d-q/2)-cbrt(d+q/2);
  } else
    return NAN;
}

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
  ra=cubicsolve(b*b,-2*MG*(b*b)/(va*va));
  rp=ra*ra/(2*MG/(va*va)-ra);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  vp=va*ra/rp;
}

void EllipseOrbit::calcbvp() {
  rp=cubicsolve(b*b,-2*MG*(b*b)/(vp*vp));
  ra=rp*rp/(2*MG/(vp*vp)-rp);
  a=(rp+ra)/2;
  t=M_2_PI*a*sqrt(a/MG);
  e=(a-rp)/a;
  va=vp*rp/ra;
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

ParabolicOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!isnan(rp))
    co.setradius(rp);
  return co;
}

ParabolicOrbit::operator EllipseOrbit() const {
  EllipseOrbit e(c);
  if(!isnan(rp))
    e.setperigey(rp);
  return e;
}

ParabolicOrbit::operator HyperbolicOrbit() const {
  HyperbolicOrbit h(c);
  if(!isnan(rp))
    h.setperigey(rp);
  if(!isnan(vp))
    h.setpspeed(vp);
  return h;
}

void ParabolicOrbit::setperigey(Real p) {
  rp=p;
  vp=sqrt(2*MG/rp);
}

void ParabolicOrbit::setpspeed(Real s) {
  vp=s;
  rp=2*MG/(rp*rp);
}

bool ParabolicOrbit::legal() const {
  if(!isnan(rp) && rp<c.radius)
    return false;
  return true;
}

void HyperbolicOrbit::calcde() {
  rp=d*sqrt((e-1.)/(e+1.));
  vp=sqrt(MG*(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcdrp() {
  e=(d*d+rp*rp)/(d*d-rp*rp);
  t=2*asin(1./e);
  vp=sqrt(MG/(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcdvi() {
  Real f=MG/(vi*d);
  vp=f+hypot(f,vi);
  rp=2*MG/(vp*vp-vi*vi);
  e=(vp*vp+vi*vi)/(vp*vp-vi*vi);
  t=2*asin(1./e);
}

void HyperbolicOrbit::calcdvp() {
  // Здесь нужно решать кубическое уравнение
  vi=cubicsolve(-vp*vp,2*MG*vp/d);
  if(isnan(vi)) // Если более одного корня, оставляем значения неопределенными
    d=NAN;
  else {
    rp=2*MG/(vp*vp-vi*vi);
    e=(vp*vp+vi*vi)/(vp*vp-vi*vi);
    t=2*asin(1./e);
  }
}

void HyperbolicOrbit::calcerp() {
  d=rp*sqrt((e+1.)/(e-1.));
  vp=sqrt(MG/(e+1.)/rp);
  vi=sqrt(MG*(e-1.)/rp);
}

void HyperbolicOrbit::calcevi() {
  vp=vi*sqrt((e+1.)/(e-1.));
  rp=2*MG/(vp*vp-vi*vi);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcevp() {
  vi=vp*sqrt((e-1.)/(e+1.));
  rp=2*MG/(vp*vp-vi*vi);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcrpvi() {
  vp=sqrt(vi*vi+2*MG/rp);
  e=(vp*vp+vi*vi)/(vp*vp-vi*vi);
  t=2*asin(1./e);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcrpvp() {
  vi=sqrt(vp*vp-2*MG/rp);
  e=(vp*vp+vi*vi)/(vp*vp-vi*vi);
  t=2*asin(1./e);
  d=rp*vp/vi;
}

void HyperbolicOrbit::calcvpvi() {
  rp=2*MG/(vp*vp-vi*vi);
  e=(vp*vp+vi*vi)/(vp*vp-vi*vi);
  t=2*asin(1./e);
  d=rp*vp/vi;
}

HyperbolicOrbit::operator CircleOrbit() const {
  CircleOrbit co(c);
  if(!isnan(rp))
    co.setradius(rp);
  else if(!isnan(vp))
    co.setspeed(vp);
  else if(!isnan(vi))
    co.setspeed(vi);
  return co;
}

HyperbolicOrbit::operator ParabolicOrbit() const {
  ParabolicOrbit p(c);
  if(!isnan(rp))
    p.setperigey(rp);
  else if(!isnan(vp))
    p.setpspeed(vp);
  else if(!isnan(vi))
    p.setpspeed(vi);
  return p;
}

HyperbolicOrbit::operator EllipseOrbit() const {
  EllipseOrbit o(c);
  if(!isnan(rp))
    o.setperigey(rp);
  if(!isnan(e))
    o.setexcentrisitet(e>1.?1./e:e);
  else if(!isnan(vp))
    o.setpspeed(vp);
  return o;
}

void HyperbolicOrbit::setperigey(Real r) {
  rp=r;
  if(!isnan(vp))
    calcrpvp();
  else if(!isnan(vi))
    calcrpvi();
  else if(!isnan(e))
    calcerp();
  else if(!isnan(d))
    calcdrp();
}

void HyperbolicOrbit::setpspeed(Real s) {
  vp=s;
  if(!isnan(rp))
    calcrpvp();
  else if(!isnan(vi))
    calcvpvi();
  else if(!isnan(e))
    calcevp();
  else if(!isnan(d))
    calcdvp();
}

void HyperbolicOrbit::setispeed(Real s) {
  vi=s;
  if(!isnan(rp))
    calcrpvi();
  else if(!isnan(vp))
    calcvpvi();
  else if(!isnan(e))
    calcevi();
  else if(!isnan(d))
    calcdvi();
}

void HyperbolicOrbit::setturn(Real _t) {
  t=_t;
  e=1./sin(t/2.);
  if(!isnan(rp))
    calcerp();
  else if(!isnan(vp))
    calcevp();
  else if(!isnan(vi))
    calcevi();
  else if(!isnan(d))
    calcde();
}

void HyperbolicOrbit::setgoal(Real g) {
  d=g;
  if(!isnan(rp))
    calcdrp();
  else if(!isnan(vp))
    calcdvp();
  else if(!isnan(vi))
    calcdvi();
  else if(!isnan(e))
    calcde();
}

void HyperbolicOrbit::setexcentrisitet(Real _e) {
  e=_e;
  t=2*asin(1./e);
  if(!isnan(rp))
    calcerp();
  else if(!isnan(vp))
    calcevp();
  else if(!isnan(vi))
    calcevi();
  else if(!isnan(d))
    calcde();
}

bool HyperbolicOrbit::legal() const {
  if(!isnan(rp) && rp<c.radius)
    return false;
  if(!isnan(e) && e<1.)
    return false;
  if(!isnan(vp) && vp<=0)
    return false;
  if(!isnan(vi) && vi<0)
    return false;
  if(!isnan(t) && (t<=0 || t>M_PI))
    return false;
  return true;
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
    cu->add(p,v);
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
    b.mass=massunit(p);
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.radius=sizeunit(p);
    p=strtok(NULL,",\n");
    if(!p) continue;
    b.period=timeunit(p);
    lstbodies.push_back(b);
  }
  fclose(f);
  for(auto i=lstbodies.begin(); i!=lstbodies.end(); ++i) {
    skybodies[(*i).enname]=&*i;
    skybodies[(*i).runame]=&*i;
  }
}

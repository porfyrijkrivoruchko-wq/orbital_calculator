#include <string.h>
#include <list>
#include <iostream>
#include "skymech.hpp"

std::map<std::string,Unit*> parms{
  {"height",&sizeunit},
  {"radius",&sizeunit},
  {"perigey",&sizeunit},
  {"apogey",&sizeunit},
  {"bigsize",&sizeunit},
  {"smallsize",&sizeunit},
  {"excentrisitet",NULL},
  {"speed",&speedunit},
  {"pspeed",&speedunit},
  {"aspeed",&speedunit},
  {"ispeed",&speedunit},
  {"pediod",&timeunit},
  {"turn",&angleunit}
};

std::ostream& operator<<(std::ostream& out, const CircleOrbit& c) {
  if(!isnan(c.radius())) {
    out << "height=" << c.radius()-c.center().radius << " м\n";
    out << "radius=" << c.radius() << " м\n";
  }
  else
    out << "radius undefined\n";
  if(!isnan(c.speed()))
    out << "speed=" << c.speed() << " м/с\n";
  else
    out << "speed undefined\n";
  if(!isnan(c.period()))
    out << "period=" << c.period() << " c\n";
  else
    out << "period undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const EllipseOrbit& e) {
  if(!isnan(e.perigey()))
    out << "perigey=" << e.perigey()-e.center().radius << " м\n";
  else
    out << "perigey undefined\n";
  if(!isnan(e.apogey()))
    out << "apogey=" << e.apogey()-e.center().radius << " м\n";
  else
    out << "apogey undefined\n";
  if(!isnan(e.bigsize()))
    out << "big size=" << e.bigsize() << " м\n";
  else
    out << "big size undefined\n";
  if(!isnan(e.smallsize()))
    out << "small size=" << e.smallsize() << " м\n";
  else
    out << "small size undefined\n";
  if(!isnan(e.excentrisitet()))
    out << "excentrisitet=" << e.excentrisitet() << '\n';
  else
    out << "excentrisitet undefined\n";
  if(!isnan(e.period()))
    out << "period=" << e.period() << " c\n";
  else
    out << "period undefined\n";
  if(!isnan(e.pspeed()))
    out << "speed in perigey=" << e.pspeed() << " м/с\n";
  else
    out << "speed in perigey undefined\n";
  if(!isnan(e.aspeed()))
    out << "speed in apogey=" << e.aspeed() << " м/с\n";
  else
    out << "speed in apogey undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const ParabolicOrbit& p) {
  if(!isnan(p.perigey()))
    out << "perigey=" << p.perigey()-p.center().radius << " м\n";
  else
    out << "perigey undefined\n";
  if(!isnan(p.pspeed()))
    out << "speed in perigey=" << p.pspeed() << " м/с\n";
  else
    out << "speed in perigey undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const HyperbolicOrbit& h) {
  if(!isnan(h.perigey()))
    out << "perigey=" << h.perigey()-h.center().radius << " м\n";
  else
    out << "perigey undefined\n";
  if(!isnan(h.excentrisitet()))
    out << "excentrisitet=" << h.excentrisitet() << '\n';
  else
    out << "excentrisitet undefined\n";
  if(!isnan(h.pspeed()))
    out << "speed in perigey=" << h.pspeed() << " м/с\n";
  else
    out << "speed in perigey undefined\n";
  if(!isnan(h.ispeed()))
    out << "speed in infinity=" << h.ispeed() << " м/с\n";
  else
    out << "speed in infinity undefined\n";
  if(!isnan(h.turn()))
    out << "turn=" << h.turn() << " рад\n";
  else
    out << "turn undefined\n";
  if(!isnan(h.goal()))
    out << "goal=" << h.goal() << " м\n";
  else
    out << "goal undefined\n";
  return out;
}

void help() {
  std::cout << "Орбитальный калькулятор версия 0.01\n"
  "Использование: " << program_invocation_short_name << 
  " [body] [-c|e|p|h] [parm=value[unit] ...] [--help]\n"
  "body\tНебесное тело - центр орбиты\n"
  "-c|e|p|h\tТип орбиты: круговая, эллиптическая, параболическая, гиперболическая\n"
  "--help\tэтот текст\n"
  "Параметры круговой орбиты:\n"
  "height\tвысота (расстояние от поверхности)\n"
  "radius\tрадиус (расстояние от центра)\n"
  "period\tпериод\n"
  "speed\tскорость\n"
  "Параметры эллиптической орбиты:\n"
  "perigey\tвысота в перигее\n"
  "apogey\tвысота в апогее\n"
  "bigsize\tбольшая полуось\n"
  "smallsize\tмалая полуось\n"
  "excentrisitet\tэксцентриситет\n"
  "pspeed\tскорость в перигее\n"
  "aspeed\tскорость в апогее\n"
  "period\tпериод\n"
  "Параметры параболической орбиты:"
  "perigey\tвысота в перигее\n"
  "pspeed\tскорость в перигее\n"
  "Параметры гиперболической орбиты:"
  "perigey\tвысота в перигее\n"
  "pspeed\tскорость в перигее\n"
  "ispeed\tскорость на бесконечности\n"
  "turn\nугол поворота\t"
  "goal\nприцельный параметр\t"
  "excentrisitet\tэксцентриситет\n";
}

int main(int argc, const char* argv[]) {
  if(argc<2) {
    help();
    return 0;
  }
  std::list<std::pair<std::string,Real> > lp;
  Orbit_type t=CIRCLE_ORBITE;
  Skybody *body=skybodies["earth"];
  for(int i=1; i<argc; i++) 
    if(!strcmp(argv[i],"-c")) t=CIRCLE_ORBITE;
    else if(!strcmp(argv[i],"-e")) t=ELLIPSE_ORBIT;
    else if(!strcmp(argv[i],"-p")) t=PARABOLIC_ORBIT;
    else if(!strcmp(argv[i],"-h")) t=HYPERBOLIC_ORBIT;
    else if(!strcmp(argv[i],"--help")) {
      help();
      return 0;
    }
    else {
      const char* p=strchr(argv[i],'=');
      if(p) {
        char name[p-argv[i]+1];
        strncpy(name,argv[i],sizeof(name)-1);
        name[sizeof(name)-1]=0;
        if(parms.count(name))
          lp.push_back(std::pair<std::string,Real>{name, parms[name]?(*parms[name])(p+1):atof(p+1)});
      } else body=skybodies[argv[i]];
    }
  switch(t) {
    case CIRCLE_ORBITE: {
      CircleOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); i++)
        if((*i).first=="radius")
          orbit.setradius((*i).second);
        else if((*i).first=="height")
          orbit.setradius((*i).second+orbit.center().radius);
        else if((*i).first=="period")
          orbit.setperiod((*i).second);
        else if((*i).first=="speed")
          orbit.setspeed((*i).second);
      std::cout << orbit << '\n';
      break;
    }
    case ELLIPSE_ORBIT: {
      EllipseOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey((*i).second+orbit.center().radius);
        else if((*i).first=="apogey")
          orbit.setapogey((*i).second+orbit.center().radius);
        else if((*i).first=="bigsize")
          orbit.setbigsize((*i).second);
        else if((*i).first=="smallsize")
          orbit.setbigsize((*i).second);
        else if((*i).first=="excentrisitet")
          orbit.setexcentrisitet((*i).second);
        else if((*i).first=="period")
          orbit.setperiod((*i).second);
        else if((*i).first=="pspeed")
          orbit.setpspeed((*i).second);
        else if((*i).first=="aspeed")
          orbit.setaspeed((*i).second);
      std::cout << orbit << '\n';
      break;
    }
    case PARABOLIC_ORBIT: {
      ParabolicOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey((*i).second+orbit.center().radius);
        else if((*i).first=="pspeed")
          orbit.setpspeed((*i).second);
      std::cout << orbit << '\n';
      break;
    }
    case HYPERBOLIC_ORBIT: {
      HyperbolicOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey((*i).second+orbit.center().radius);
        else if((*i).first=="pspeed")
          orbit.setpspeed((*i).second);
        else if((*i).first=="ispeed")
          orbit.setispeed((*i).second);
        else if((*i).first=="excentrisitet")
          orbit.setexcentrisitet((*i).second);
        else if((*i).first=="turn")
          orbit.setturn((*i).second);
        else if((*i).first=="goal")
          orbit.setgoal((*i).second);
      std::cout << orbit << '\n';
      break;
    }
  }
  return 0;
}

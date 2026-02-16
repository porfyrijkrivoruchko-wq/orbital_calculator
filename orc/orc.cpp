#include <string.h>
#include <list>
#include <vector>
#include <iostream>
#include "skymech.hpp"

std::map<std::string,Unit*> parms{
  {"height",&sizeunit},
  {"radius",&sizeunit},
  {"perigey",&sizeunit},
  {"apogey",&sizeunit},
  {"bigsize",&sizeunit},
  {"smallsize",&sizeunit},
  {"distance",&sizeunit},
  {"excentrisitet",NULL},
  {"speed",&speedunit},
  {"pspeed",&speedunit},
  {"aspeed",&speedunit},
  {"ispeed",&speedunit},
  {"period",&timeunit},
  {"turn",&angleunit},
  {"tspeed",&speedunit},
  {"nspeed",&speedunit},
  {"aiming",&sizeunit},
  {"pitch",&angleunit}
};

std::ostream& operator<<(std::ostream& out, const mass_value<Real>& m) {
  out << massunit(m.value());
  return out;
}

std::ostream& operator<<(std::ostream& out, const distance_value<Real>& d) {
  out << sizeunit(d.value());
  return out;
}

std::ostream& operator<<(std::ostream& out, const speed_value<Real>& v) {
  out << speedunit(v.value());
  return out;
}

std::ostream& operator<<(std::ostream& out, const time_value<Real>& t) {
  out << timeunit(t.value());
  return out;
}

std::ostream& operator<<(std::ostream& out, const CircleOrbit& c) {
  if(!c.radius().empty()) {
    out << "height=" << (c.radius()-c.center().radius) << '\n';
    out << "radius=" << c.radius() << '\n';
  }
  else
    out << "radius undefined\n";
  if(!c.speed().empty())
    out << "speed=" << c.speed() << '\n';
  else
    out << "speed undefined\n";
  if(!c.period().empty())
    out << "period=" << c.period() << '\n';
  else
    out << "period undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const EllipseOrbit& e) {
  if(!e.perigey().empty())
    out << "perigey=" << (e.perigey()-e.center().radius) << '\n';
  else
    out << "perigey undefined\n";
  if(!e.apogey().empty())
    out << "apogey=" << (e.apogey()-e.center().radius) << '\n';
  else
    out << "apogey undefined\n";
  if(!e.bigsize().empty())
    out << "big size=" << e.bigsize() << '\n';
  else
    out << "big size undefined\n";
  if(!e.smallsize().empty())
    out << "small size=" << e.smallsize() << '\n';
  else
    out << "small size undefined\n";
  if(!isnan(e.excentrisitet()))
    out << "excentrisitet=" << e.excentrisitet() << '\n';
  else
    out << "excentrisitet undefined\n";
  if(!e.period().empty())
    out << "period=" << e.period() << '\n';
  else
    out << "period undefined\n";
  if(!e.pspeed().empty())
    out << "speed in perigey=" << e.pspeed() << '\n';
  else
    out << "speed in perigey undefined\n";
  if(!e.aspeed().empty())
    out << "speed in apogey=" << e.aspeed() << '\n';
  else
    out << "speed in apogey undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const ParabolicOrbit& p) {
  if(!p.perigey().empty())
    out << "perigey=" << (p.perigey()-p.center().radius) << '\n';
  else
    out << "perigey undefined\n";
  if(!p.pspeed().empty())
    out << "speed in perigey=" << p.pspeed() << '\n';
  else
    out << "speed in perigey undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const HyperbolicOrbit& h) {
  if(!h.perigey().empty())
    out << "perigey=" << (h.perigey()-h.center().radius) << '\n';
  else
    out << "perigey undefined\n";
  if(!isnan(h.excentrisitet()))
    out << "excentrisitet=" << h.excentrisitet() << '\n';
  else
    out << "excentrisitet undefined\n";
  if(!h.pspeed().empty())
    out << "speed in perigey=" << h.pspeed() << '\n';
  else
    out << "speed in perigey undefined\n";
  if(!h.ispeed().empty())
    out << "speed in infinity=" << h.ispeed() << '\n';
  else
    out << "speed in infinity undefined\n";
  if(!isnan(h.turn()))
    out << "turn=" << angleunit(h.turn()) << '\n';
  else
    out << "turn undefined\n";
  if(!h.aiming().empty())
    out << "aiming=" << h.aiming() << '\n';
  else
    out << "aiming undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const Skybody& b) {
  out << "Mass=" << b.mass << "\nRadius=" << b.radius << "\nPeriod=" << b.period << '\n';
  return out;
}

std::ostream& operator<<(std::ostream& out, const Point& p) {
  if(p.distance().empty())
    out << "distance undefined\n";
  else
    out << "distance=" << p.distance() << '\n';
  if(p.aiming().empty())
    out << "aiming undefined\n";
  else
    out << "aiming=" << p.aiming() << '\n';
  if(!isnan(p.pitch()))
    out << "pitch=" << angleunit(p.pitch()) << '\n';
  else
    out << "pitch undefined\n";
  if(p.speed().empty())
    out << "speed undefined\n";
  else
    out << "speed=" << p.speed() << '\n';
  if(p.tspeed().empty())
    out << "tspeed undefined\n";
  else
    out << "tspeed=" << p.tspeed() << '\n';
  if(p.nspeed().empty())
    out << "nspeed undefined\n";
  else
    out << "nspeed=" << p.nspeed() << '\n';
  const Orbit *orb=p.orbit();
  if(!orb)
    out << "orbit undefined\n";
  else switch(orb->type()) {
    case CIRCLE_ORBITE:
      out << "Circle orbit\n" << *(static_cast<const CircleOrbit*>(orb));
      break;
    case ELLIPSE_ORBIT:
      out << "Ellipse orbit\n" << *(static_cast<const EllipseOrbit*>(orb));
      break;
    case PARABOLIC_ORBIT:
      out << "Parabolic orbit\n" << *(static_cast<const ParabolicOrbit*>(orb));
      break;
    case HYPERBOLIC_ORBIT:
      out << "Hyperbolic orbit\n" << *(static_cast<const HyperbolicOrbit*>(orb));
      break;
  }
  return out;
}

extern std::vector<Skybody> lstbodies;

void help(char target=0) {
  std::cout << "Орбитальный калькулятор версия 0.03\n";
  switch(target) {
    case 0:
    std::cout << "Использование: " << program_invocation_short_name << 
    " [body] [-u{m|d|t|s|a} value ...] [-c|e|p|h|i|v] [parm=value[unit] ...] [--help[=bodies|units]]\n"
    "body\tНебесное тело - центр орбиты\n"
    "-c|e|p|h\tТип орбиты: круговая (по умолчанию), эллиптическая, параболическая, гиперболическая\n"
    "-i\tВывести информацию о небесном теле\n"
    "-v\tОпределить орбиту по вектору скорости\n"
    "--help\tэтот текст\n"
    "--help=bodies\tдоступные небесные тела\n"
    "--help=units\tдоступные единицы измерения\n"
    "-m{m|d|t|s|a} установить единицы измерения массы, расстояний, времени, скорости и углов соответственно\n"
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
    "Параметры гиперболической орбиты:\n"
    "perigey\tвысота в перигее\n"
    "pspeed\tскорость в перигее\n"
    "ispeed\tскорость на бесконечности\n"
    "turn\tугол поворота\n"
    "aiming\tприцельный параметр (на бесконечности)\n"
    "excentrisitet\tэксцентриситет\n"
    "Параметры вектора скорости:\n"
    "distance\tрасстояние\n"
    "speed\tмодуль скорости\n"
    "tspead\tтангенциальная скорость\n"
    "nspeed\tнормальная скорость\n"
    "aiming\tприцельный параметр\n"
    "pitch\tтангаж (угол между вектором скорости и касательной к окружности)\n";
    break;
    case 'b':
      std::cout << "Доступные небесные тела:\n";
      for(auto i=lstbodies.begin(); i!=lstbodies.end(); ++i)
        std::cout << (*i).enname << '\t' << (*i).runame << '\n';
      break;
    case 'u':
      std::cout << "Единицы изменения:\nМасса:";
      for(auto i=massunit.begin(); i!=massunit.end(); ++i) {
        std::cout << ' ' << (*i).first;
        if((*i).first==massunit.getdflt())
          std::cout << '*';
      }
      std::cout << "\nРасстояние:";
      for(auto i=sizeunit.begin(); i!=sizeunit.end(); ++i) {
        std::cout << ' ' << (*i).first;
        if((*i).first==sizeunit.getdflt())
          std::cout << '*';
      }
      std::cout << "\nВремя:";
      for(auto i=timeunit.begin(); i!=timeunit.end(); ++i) {
        std::cout << ' ' << (*i).first;
        if((*i).first==timeunit.getdflt())
          std::cout << '*';
      }
      std::cout << "\nСкорость:";
      for(auto i=speedunit.begin(); i!=speedunit.end(); ++i) {
        std::cout << ' ' << (*i).first;
        if((*i).first==speedunit.getdflt())
          std::cout << '*';
      }
      std::cout << "\nУгол:";
      for(auto i=angleunit.begin(); i!=angleunit.end(); ++i) {
        std::cout << ' ' << (*i).first;
        if((*i).first==angleunit.getdflt())
          std::cout << '*';
      }
      std::cout << '\n';
      break;
  }
}

int main(int argc, const char* argv[]) {
  if(argc<2) {
    help();
    return 0;
  }
  std::list<std::pair<std::string,Real> > lp;
  Orbit_type t=CIRCLE_ORBITE;
  Skybody *body=skybodies["earth"];
  bool info=false, vect=false;
  for(int i=1; i<argc; i++) 
    if(!strcmp(argv[i],"-c")) t=CIRCLE_ORBITE;
    else if(!strcmp(argv[i],"-e")) t=ELLIPSE_ORBIT;
    else if(!strcmp(argv[i],"-p")) t=PARABOLIC_ORBIT;
    else if(!strcmp(argv[i],"-h")) t=HYPERBOLIC_ORBIT;
    else if(!strcmp(argv[i],"-i")) info=true;
    else if(!strcmp(argv[i],"-v")) vect=true;
    else if(!strcmp(argv[i],"-um"))
      massunit.setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-ud"))
      sizeunit.setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-ut"))
      timeunit.setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-us"))
      speedunit.setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-ua"))
      angleunit.setdflt(argv[++i]);
    else if(!strcmp(argv[i],"--help")) {
      help();
      return 0;
    } else if(!strcmp(argv[i],"--help=bodies")) {
      help('b');
      return 0;
    } else if(!strcmp(argv[i],"--help=units")) {
      help('u');
      return 0;
    }
    else {
      const char* p=strchr(argv[i],'=');
      if(p) {
        char name[p++-argv[i]+1];
        strncpy(name,argv[i],sizeof(name)-1);
        name[sizeof(name)-1]=0;
        if(parms.count(name))
          lp.push_back(std::pair<std::string,Real>{name, parms[name]?(*parms[name])(p):atof(p)});
        else {
          std::cerr << "Неизвестный параметр " << name << '\n';
          return 1;
        }
      } else if(skybodies.count(argv[i]))
        body=skybodies[argv[i]];
      else {
        std::cerr << "Неизвестное тело " << argv[i] << '\n';
        return 1;
      }
    }
  if(info)
    std::cout << *body << '\n';
  if(vect) {
    Point p(*body);
    for(auto i=lp.begin(); i!=lp.end(); ++i)
      if((*i).first=="distance")
        p.setdistance(distance_value<Real>((*i).second));
      else if((*i).first=="speed")
        p.setspeed(speed_value<Real>((*i).second));
    else if((*i).first=="tspeed")
      p.settspeed(speed_value<Real>((*i).second));
    else if((*i).first=="nspeed")
      p.setnspeed(speed_value<Real>((*i).second));
    else if((*i).first=="aiming")
      p.setaiming(distance_value<Real>((*i).second));
    else if((*i).first=="pitch")
      p.setpitch((*i).second);
    std::cout << p << '\n';
  } else
  switch(t) {
    case CIRCLE_ORBITE: {
      CircleOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); i++)
        if((*i).first=="radius")
          orbit.setradius(distance_value<Real>((*i).second));
        else if((*i).first=="height")
          orbit.setradius(distance_value<Real>((*i).second)+orbit.center().radius);
        else if((*i).first=="period")
          orbit.setperiod(time_value<Real>((*i).second));
        else if((*i).first=="speed")
          orbit.setspeed(speed_value<Real>((*i).second));
      std::cout << orbit << '\n';
      break;
    }
    case ELLIPSE_ORBIT: {
      EllipseOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey(distance_value<Real>((*i).second)+orbit.center().radius);
        else if((*i).first=="apogey")
          orbit.setapogey(distance_value<Real>((*i).second)+orbit.center().radius);
        else if((*i).first=="bigsize")
          orbit.setbigsize(distance_value<Real>((*i).second));
        else if((*i).first=="smallsize")
          orbit.setsmallsize(distance_value<Real>((*i).second));
        else if((*i).first=="excentrisitet")
          orbit.setexcentrisitet((*i).second);
        else if((*i).first=="period")
          orbit.setperiod(time_value<Real>((*i).second));
        else if((*i).first=="pspeed")
          orbit.setpspeed(speed_value<Real>((*i).second));
        else if((*i).first=="aspeed")
          orbit.setaspeed(speed_value<Real>((*i).second));
      std::cout << orbit << '\n';
      break;
    }
    case PARABOLIC_ORBIT: {
      ParabolicOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey(distance_value<Real>((*i).second)+orbit.center().radius);
        else if((*i).first=="pspeed")
          orbit.setpspeed(speed_value<Real>((*i).second));
      std::cout << orbit << '\n';
      break;
    }
    case HYPERBOLIC_ORBIT: {
      HyperbolicOrbit orbit(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="perigey")
          orbit.setperigey(distance_value<Real>((*i).second)+orbit.center().radius);
        else if((*i).first=="pspeed")
          orbit.setpspeed(speed_value<Real>((*i).second));
        else if((*i).first=="ispeed")
          orbit.setispeed(speed_value<Real>((*i).second));
        else if((*i).first=="excentrisitet")
          orbit.setexcentrisitet((*i).second);
        else if((*i).first=="turn")
          orbit.setturn((*i).second);
        else if((*i).first=="aiming")
          orbit.setaiming(distance_value<Real>((*i).second));
      std::cout << orbit << '\n';
      break;
    }
  }
  return 0;
}

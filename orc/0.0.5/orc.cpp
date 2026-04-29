#include <string.h>
#include <list>
#include <vector>
#include <iostream>
#include "skymech.hpp"

std::map<std::string,std::string> parms{
  {"height","distance"},
  {"radius","distance"},
  {"perigey","distance"},
  {"apogey","distance"},
  {"bigsize","distance"},
  {"smallsize","distance"},
  {"distance","distance"},
  {"excentrisitet",""},
  {"speed","speed"},
  {"pspeed","speed"},
  {"aspeed","speed"},
  {"ispeed","speed"},
  {"period","time"},
  {"turn","angle"},
  {"tspeed","speed"},
  {"nspeed","speed"},
  {"aiming","distance"},
  {"pitch","angle"},
  {"baseheight","distance"},
  {"baseradius","distance"},
  {"basespeed","speed"},
  {"baseperiod","time"},
  {"length","distance"},
  {"lowheight","distance"},
  {"highheight","distance"},
  {"lowradius","distance"},
  {"highradius","distance"},
  {"lowmass","mass"},
  {"highmass","mass"},
  {"transmass","mass"},
  {"position","distance"},
  {"cordmass","mass"},
  {"denscord","density"},
  {"stcord","pressure"},
  {"seccord","area"},
  {"drymass","mass"},
  {"fuel","mass"},
  {"fullmass","mass"},
  {"spimp","speed"},
  {"trforce","force"}
};

std::ostream& operator<<(std::ostream& out, const CircleOrbit& c) {
  if(!c.radius().empty()) {
    out << "height=" << c.radius()-c.center().radius << '\n';
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
    out << "perigey=" << e.perigey()-e.center().radius << '\n';
  else
    out << "perigey undefined\n";
  if(!e.apogey().empty())
    out << "apogey=" << e.apogey()-e.center().radius << '\n';
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
    out << "perigey=" << p.perigey()-p.center().radius << '\n';
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
    out << "perigey=" << distance_value<Real>(h.perigey()-h.center().radius) << '\n';
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
    out << "turn=" << units["angle"](h.turn()) << '\n';
  else
    out << "turn undefined\n";
  if(!h.aiming().empty())
    out << "aiming=" << h.aiming() << '\n';
  else
    out << "aiming undefined\n";
  return out;
}

std::ostream& operator<<(std::ostream& out, const Skybody& b) {
  out << "Mass=" << b.mass << "\nRadius=" << b.radius << "\nPeriod=" << b.period << "\nVolume=" << b.volume() << "\nSurface=" << b.surface() << "\nDensity=" << b.density() << "\nEquator gravity=" << b.gravity(0) << "\nPolar gravity=" << b.gravity(M_PI_2) << '\n';
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
    out << "pitch=" << units["angle"](p.pitch()) << '\n';
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
  if(isnan(p.argument()))
    out << "argument undefined\n";
  else
    out << "argument=" << p.argument() << '\n';
  if(p.time().empty())
    out << "time undefined\n";
  else
    out << "time=" << p.time() << '\n';
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
    default: ;
  }
  return out;
}

std::ostream& operator<<(std::ostream& out, const OrbitalLift& lift) {
  out << "Low station:\n";
  if(lift.mass(0).empty())
    out << "mass undefined\n";
  else
    out << "mass=" << lift.mass(0) << '\n';
  if(lift.radius(0).empty())
    out << "radius undefined\n";
  else
    out << "radius=" << lift.radius(0) << '\n';
  if(lift.height(0).empty())
    out << "height undefined\n";
  else
    out << "height=" << lift.height(0) << '\n';
  if(lift.speed(0).empty())
    out << "speed undefined" << '\n';
  else 
    out << "speed=" << lift.speed(0) << '\n';
  if(lift.gravity(distance_value<Real>(0.)).empty())
    out << "gravity undefined\n";
  else
    out << "gravity=" << lift.gravity(distance_value<Real>(0.)) << '\n';
  if(lift.tension(distance_value<Real>(0.)).empty())
    out << "tension undefined\n";
  else
    out << "tension=" << lift.tension(distance_value<Real>(0.)) << '\n';
  out << "High station:\n";
  if(lift.mass(1).empty())
    out << "mass undefined\n";
  else
    out << "mass=" << lift.mass(1) << '\n';
  if(lift.radius(1).empty())
    out << "radius undefined\n";
  else
    out << "radius=" << lift.radius(1) << '\n';
  if(lift.height(1).empty())
    out << "height undefined\n";
  else
    out << "height=" << lift.height(1) << '\n';
  if(lift.speed(1).empty())
    out << "speed undefined" << '\n';
  else 
    out << "speed=" << lift.speed(1) << '\n';
  if(lift.gravity(lift.length()).empty())
    out << "gravity undefined\n";
  else
    out << "gravity=" << lift.gravity(lift.length()) << '\n';
  if(lift.tension(lift.length()).empty())
    out << "tension undefined\n";
  else
    out << "tension=" << lift.tension(lift.length()) << '\n';
  out << "Transporter:\n";
  if(lift.mass(2).empty())
    out << "mass undefined\n";
  else
    out << "mass=" << lift.mass(2) << '\n';
  if(lift.position().empty())
    out << "position undefined\n";
  else
    out << "position=" << lift.position() << '\n';
  if(lift.speed(2).empty())
    out << "speed undefined\n";
  else
    out << "speed=" << lift.speed(2) << '\n';
  if(lift.gravity(lift.position()).empty())
    out << "gravity undefined\n";
  else
    out << "gravity=" << lift.gravity(lift.position()) << '\n';
  out << "Cord:\n";
  if(lift.mass(3).empty())
    out << "mass undefined\n";
  else
    out << "mass=" << lift.mass(3) << '\n';
  if(lift.length().empty())
    out << "length undefined\n";
  else
    out << "length=" << lift.length() << '\n';
  if(lift.strength().empty())
    out << "strength undefined\n";
  else
    out << "strength=" << lift.strength() << '\n';
  if(lift.cross_section().empty())
    out << "cross section undefined\n";
  else
    out << "cross section=" << lift.cross_section();
  if(lift.cable_density().empty())
    out << "density undefined\n";
  else
    out << "density=" << lift.cable_density() << '\n';
  if(lift.period().empty())
    out << "period undefined\n";
  else
    out << "period=" << lift.period() << '\n';
  return out;
}

void help(char target=0) {
  std::cout << "Орбитальный калькулятор версия 0.05\n";
  switch(target) {
    case 0:
    std::cout << "Использование: " << program_invocation_short_name << 
    " [body] [-u{m|d|t|s|a} value ...] [-{c|e|p|h|i|v|l}] [parm=expression ...] [--help[=bodies|units]]\n"
    "body\tНебесное тело - центр орбиты\n"
    "-c|e|p|h\tТип орбиты: круговая (по умолчанию), эллиптическая, параболическая, гиперболическая\n"
    "-i\tВывести информацию о небесном теле\n"
    "-v\tОпределить орбиту по вектору скорости\n"
    "-l\tМежорбитальный лифт\n"
    "--help\tэтот текст\n"
    "--help=bodies\tдоступные небесные тела\n"
    "--help=units\tдоступные единицы измерения\n"
    "--help=circle\tпараметры круговой орбиты\n"
    "--help=ellipse\tпараметры эллиптической орбиты\n"
    "--help=parabola\tпараметры параболической траектории\n"
    "--help=hyperbola\tпараметры гиперболической траектории\n"
    "--help=vector\tпараметры вектора скорости\n"
    "--help=base\tпараметры опорной круговой орбиты\n"
    "--help=lift\tпараметры орбитального лифта\n"
    "--help=engine\tпараметры двигательной установки\n"
    "-u{m|d|t|s|a} установить единицы измерения массы, расстояний, времени, скорости и углов соответственно\n";
    break;
    case 'c':
    std::cout << "Параметры круговой орбиты:\n"
    "height\tвысота (расстояние от поверхности)\n"
    "radius\tрадиус (расстояние от центра)\n"
    "period\tпериод\n"
    "speed\tскорость\n";
    break;
    case 'e':
    std::cout << "Параметры эллиптической орбиты:\n"
    "perigey\tвысота в перигее\n"
    "apogey\tвысота в апогее\n"
    "bigsize\tбольшая полуось\n"
    "smallsize\tмалая полуось\n"
    "excentrisitet\tэксцентриситет\n"
    "pspeed\tскорость в перигее\n"
    "aspeed\tскорость в апогее\n"
    "period\tпериод\n";
    break;
    case 'p':
    std::cout << "Параметры параболической орбиты:"
    "perigey\tвысота в перигее\n"
    "pspeed\tскорость в перигее\n";
    break;
    case 'h':
    std::cout << "Параметры гиперболической орбиты:\n"
    "perigey\tвысота в перигее\n"
    "pspeed\tскорость в перигее\n"
    "ispeed\tскорость на бесконечности\n"
    "turn\tугол поворота\n"
    "aiming\tприцельный параметр (на бесконечности)\n"
    "excentrisitet\tэксцентриситет\n";
    break;
    case 'v':
    std::cout << "Параметры вектора скорости:\n"
    "distance\tрасстояние\n"
    "speed\tмодуль скорости\n"
    "tspeed\tтангенциальная скорость\n"
    "nspeed\tнормальная скорость\n"
    "aiming\tприцельный параметр\n"
    "pitch\tтангаж (угол между вектором скорости и касательной к окружности)\n";
    break;
    case 'B':
    std::cout << "Параметры опорной круговой орбиты\n(если заданы, вычисляется двухимпульсный маневр перехода с опорной на целевую орбиту)\n"
    "baseheight\tВысота\n"
    "baseradius\tрадиус\n"
    "basespeed\tскорость\n"
    "baseperiod\tпериод\n";
    break;
    case 'l':
    std::cout << "Параметры орбитального лифта:\n"
    "length\tдлина троса\n"
    "lowheight\tвысота нижней станции\n"
    "highheight\tвысота верхней станции\n"
    "lowradius\tрадиус обращения нижней станции\n"
    "highradius\tрадиус обращения верхней станции\n"
    "lowmass\tмасса нижней станции\n"
    "highmass\tмасса верхней станции\n"
    "transmass\tмасса транспортера\n"
    "position\tпозиция транспортера\n"
    "cordmass\tмасса троса\n"
    "denscord\tплотность троса\n"
    "stcord\tудельная прочность троса\n"
    "seccord\tсечение троса\n";
    case 'E':
    std::cout << "Параметры двигательной установки:\n"
    "drymass\tсухая масса аппарата\n"
    "fuel\tмасса топлива\n"
    "fullmass\tполная масса аппарата\n"
    "spimp\tудельный импульс (в ед. скорости)\n"
    "trforce\tсила тяги\n";
    break;
    case 'b':
      std::cout << "Доступные небесные тела:\n";
      for(auto i=lstbodies.begin(); i!=lstbodies.end(); ++i)
        std::cout << (*i).enname << '\t' << (*i).runame << '\n';
      break;
    case 'u':
      std::cout << "Единицы изменения:\n";
      for(auto i=units.begin(); i!=units.end(); ++i) {
        std::cout << (*i).first << ' ' << (*i).second.name() << ":\n";
        for(auto j=(*i).second.begin(); j!=(*i).second.end(); ++j) {
          std::cout << ' ' << (*j).first;
          if((*j).first==(*i).second.getdflt())
            std::cout << '*';
        }
        std::cout << '\n';
      }
      break;
  }
}

int main(int argc, const char* argv[]) {
  if(argc<2) {
    help();
    return 0;
  }
  std::list<std::pair<std::string,Real> > lp;
  Orbit_type t=UNNOWN_ORBIT;
  Skybody *body=skybodies["earth"];
  CircleOrbit* base=NULL;
  engine *eng=NULL;
  speed_value<Real> pulse1, pulse2;
  bool info=false, vect=false;
  for(int i=1; i<argc; i++) 
    if(!strcmp(argv[i],"-c")) t=CIRCLE_ORBITE;
    else if(!strcmp(argv[i],"-e")) t=ELLIPSE_ORBIT;
    else if(!strcmp(argv[i],"-p")) t=PARABOLIC_ORBIT;
    else if(!strcmp(argv[i],"-h")) t=HYPERBOLIC_ORBIT;
    else if(!strcmp(argv[i],"-l")) t=LIFT_ORBIT;
    else if(!strcmp(argv[i],"-i")) info=true;
    else if(!strcmp(argv[i],"-v")) vect=true;
    else if(!strcmp(argv[i],"-um"))
      units["mass"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-ud"))
      units["distance"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-ut"))
      units["time"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-us"))
      units["speed"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uan"))
      units["angle"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uac"))
      units["acselerate"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uD"))
      units["density"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uv"))
      units["volume"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uar"))
      units["area"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-uf"))
      units["force"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"-up"))
      units["pressure"].setdflt(argv[++i]);
    else if(!strcmp(argv[i],"--help"))
      help();
    else if(!strcmp(argv[i],"--help=bodies"))
      help('b');
    else if(!strcmp(argv[i],"--help=units"))
      help('u');
    else if(!strcmp(argv[i],"--help=circle"))
      help('c');
    else if(!strcmp(argv[i],"--help=ellipse"))
      help('e');
    else if(!strcmp(argv[i],"--help=parabola"))
      help('p');
    else if(!strcmp(argv[i],"--help=hyperbola"))
      help('h');
    else if(!strcmp(argv[i],"--help=lift"))
      help('l');
    else if(!strcmp(argv[i],"--help=base"))
      help('B');
    else if(!strcmp(argv[i],"--help=engine"))
      help('E');
    else {
      const char* p=strchr(argv[i],'=');
      if(p) {
        char name[p++-argv[i]+1];
        strncpy(name,argv[i],sizeof(name)-1);
        name[sizeof(name)-1]=0;
        if(parms.count(name)) {
          try {
            lp.push_back(std::pair<std::string,Real>{name, !parms[name].empty()?units[parms[name]](p):atof(p)});
          } catch(std::string msg) {
            std::cerr << "Ошибка интерпретации параметра: " << argv[i] << '\n';
            return 1;
          }
        }
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
  for(auto i=lp.begin(); i!=lp.end(); ++i)
    if((*i).first=="basedistance") {
      if(!base)
        base=new CircleOrbit(*body);
      base->setradius(distance_value<Real>((*i).second));
    } else if((*i).first=="basespeed") {
      if(!base)
        base=new CircleOrbit(*body);
      base->setspeed(speed_value<Real>((*i).second));
    } else if((*i).first=="baseheight") {
      if(!base)
        base=new CircleOrbit(*body);
      base->setradius(distance_value<Real>((*i).second)+base->center().radius);
    } else if((*i).first=="baseperiod") {
      if(!base)
        base=new CircleOrbit(*body);
      base->setperiod(time_value<Real>((*i).second));
    } else if((*i).first=="drymass") {
      if(!eng)
        eng=new engine;
      eng->setdry(mass_value<Real>((*i).second));
    } else if((*i).first=="fuel") {
      if(!eng)
        eng=new engine;
      eng->setfuel(mass_value<Real>((*i).second));
    } else if((*i).first=="fullmass") {
      if(!eng)
        eng=new engine;
      eng->setfull(mass_value<Real>((*i).second));
    } else if((*i).first=="spimp") {
      if(!eng)
        eng=new engine;
      eng->setspimp(speed_value<Real>((*i).second));
    } else if((*i).first=="trforce") {
      if(!eng)
        eng=new engine;
      eng->setforce(force_value<Real>((*i).second));
    }
  EllipseOrbit *pass=NULL;
  if(base) {
    pass=new EllipseOrbit(*body);
    pass->setperigey(base->radius());
    std::cout << "Опорная орбита: " << *base << "Целевая орбита:\n";
  }
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
    if(base && !p.orbit()->perigey().empty()) {
      pass->setapogey(p.orbit()->perigey());
      pulse1=pass->pspeed()-base->speed();
      if(!p.orbit()->pspeed().empty())
        pulse2=p.orbit()->pspeed()-pass->aspeed();
    }
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
      if(base && !orbit.radius().empty()) {
        pass->setapogey(orbit.radius());
        pulse1=pass->pspeed()-base->speed();
        pulse2=orbit.speed()-pass->aspeed();
      }
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
      if(base && !orbit.perigey().empty()) {
        pass->setapogey(orbit.perigey());
        pulse1=pass->pspeed()-base->speed();
        if(!orbit.pspeed().empty())
          pulse2=orbit.pspeed()-pass->aspeed();
      }
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
      if(base && !orbit.perigey().empty()) {
        pass->setapogey(orbit.perigey());
        pulse1=pass->pspeed()-base->speed();
        if(!orbit.pspeed().empty())
          pulse2=orbit.pspeed()-pass->aspeed();
      }
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
      if(base && !orbit.perigey().empty()) {
        pass->setapogey(orbit.perigey());
        pulse1=pass->pspeed()-base->speed();
        if(!orbit.pspeed().empty())
          pulse2=orbit.pspeed()-pass->aspeed();
      }
      break;
    }
    case LIFT_ORBIT: {
      OrbitalLift lift(*body);
      for(auto i=lp.begin(); i!=lp.end(); ++i)
        if((*i).first=="length")
          lift.setlength(distance_value<Real>((*i).second));
        else if((*i).first=="lowheight")
          lift.setheight(distance_value<Real>((*i).second),0);
        else if((*i).first=="highheight")
          lift.setheight(distance_value<Real>((*i).second),1);
        else if((*i).first=="lowradius")
          lift.setradius(distance_value<Real>((*i).second),0);
        else if((*i).first=="highradius")
          lift.setradius(distance_value<Real>((*i).second),1);
        else if((*i).first=="position")
          lift.setposition(distance_value<Real>((*i).second));
        else if((*i).first=="lowmass")
          lift.setmass(mass_value<Real>((*i).second),0);
        else if((*i).first=="highmass")
          lift.setmass(mass_value<Real>((*i).second),1);
        else if((*i).first=="transmass")
          lift.setmass(mass_value<Real>((*i).second),2);
        else if((*i).first=="cordmass")
          lift.setmass(mass_value<Real>((*i).second),3);
        else if((*i).first=="denscord")
          lift.setdensity(density_value<Real>((*i).second));
        else if((*i).first=="stcord")
          lift.setstrength(pressure_value<Real>((*i).second));
        else if((*i).first=="seccord")
          lift.setsection(area_value<Real>((*i).second));
      std::cout << lift << '\n';
      break;
    }
    default: break;
  }
  if(!pulse1.empty()) {
    std::cout << "1st pulse: " << pulse1 << '\n';
  }
  if(!pulse2.empty()) {
    std::cout << "2nd pulse: " << pulse2 << '\n';
    std::cout << "Total: " << pulse1+pulse2 << '\n';
    if(eng) {
      auto p=(*eng)(pulse1+pulse2);
      std::cout << "Fuel consumption: " << p.consumption << "\nduration impulse: " << p.duration << '\n'; 
    }
  }
  if(pass && !pass->period().empty())
    std::cout << "Pass time: " << pass->period()/2. << '\n';
  return 0;
}

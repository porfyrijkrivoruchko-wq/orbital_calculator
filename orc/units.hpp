#ifndef _UNITS_HPP
#define _UNITS_HPP

#include<map>
#include<string>

typedef double Real;

class Unit {
  std::map<std::string, Real> vlst;
  std::string dflt,nm;
public:
  Unit() {}
//  Unit(const Unit& u):vlst(u.vlst),dflt(u.dflt),nm(u.nm){}
  void setname(std::string n) {
    nm=n;
  }
  void add(std::string n, Real v) {
    vlst.insert(std::pair<std::string,Real>{n,v});
  }
  Real operator()(std::string) const;
  typedef std::map<std::string, Real>::const_iterator iterator;
  iterator operator[](std::string n) const {
    auto p=end();
    for(auto i=begin(); i!=end(); ++i)
      if(!strncmp(n.c_str(),(*i).first.c_str(),(*i).first.size()))
        p=i;
    return p;
  }
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
  std::string name() const { return nm; }
};

extern std::map<std::string,Unit> units;

#endif

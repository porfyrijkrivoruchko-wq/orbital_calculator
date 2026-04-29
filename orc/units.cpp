#include <string.h>
#include "units.hpp"

const char* parse_buf;
int buf_ptr;
const Unit* expr_unit;
Real parse_result;
int yyparse();
std::map<std::string,Unit> units;

Real Unit::operator()(std::string v) const {
  parse_buf=v.c_str();
  buf_ptr=0;
  expr_unit=this;
  yyparse();
  return parse_result;
}

std::string Unit::operator()(Real v) const {
  char buf[32];
  sprintf(buf,"%g %s", v/(*vlst.find(dflt)).second, dflt.c_str());
  return buf;
}

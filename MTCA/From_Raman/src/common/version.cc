#include "toolbox/version.h"
#include "xoap/version.h"
#include "xdaq/version.h"
#include "hcal/base/version.h"
#include "hcal/aux/version.h"
#include "hcal/uhtr/version.h"

GETPACKAGEINFO(hcalUHTR);

void hcalUHTR::checkPackageDependencies() throw (config::PackageInfo::VersionException) {
  CHECKDEPENDENCY(toolbox);
  CHECKDEPENDENCY(xdaq);
  CHECKDEPENDENCY(xoap);
  CHECKDEPENDENCY(hcalBase);
  CHECKDEPENDENCY(hcalAux);
}

std::set<std::string,std::less<std::string> > hcalUHTR::getPackageDependencies() {
  std::set<std::string,std::less<std::string> > deps;
  ADDDEPENDENCY(deps,toolbox);
  ADDDEPENDENCY(deps,xoap);
  ADDDEPENDENCY(deps,xdaq);
  ADDDEPENDENCY(deps,hcalBase);
  ADDDEPENDENCY(deps,hcalAux);
  return deps;
}

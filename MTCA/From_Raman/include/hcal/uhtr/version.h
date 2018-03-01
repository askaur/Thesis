/* -*- C++ -*- */

#ifndef hcalUHTRV_H_included
#define hcalUHTRV_H_included 1

#ifndef DOXYGEN_IGNORE_THIS

#include "config/PackageInfo.h"

namespace hcalUHTR
{
#define HCALUHTR_VERSION_MAJOR 1
#define HCALUHTR_VERSION_MINOR 7
#define HCALUHTR_VERSION_PATCH 5

  const std::string package = "hcal/uHTR";
  const std::string versions = PACKAGE_VERSION_STRING(HCALUHTR_VERSION_MAJOR,HCALUHTR_VERSION_MINOR,HCALUHTR_VERSION_PATCH);
  const std::string description = "uHTR control code";
  const std::string summary = "uHTR control code";
  const std::string authors = "S.C. Kao, J. Mans, N. Ruckstuhl";
  const std::string link = "  ";

  config::PackageInfo getPackageInfo();
  void checkPackageDependencies() throw (config::PackageInfo::VersionException);
  std::set<std::string,std::less<std::string> > getPackageDependencies();
}

#endif // DOXYGEN_IGNORE_THIS

#endif // hcalUHTRV_H_included

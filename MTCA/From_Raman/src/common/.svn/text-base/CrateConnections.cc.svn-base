#include "hcal/uhtr/CrateConnections.hh"
#include "hcal/aux/hcalCrate.hh"
#include "hcal/exception/uHALException.hh"
#include "hcal/toolbox/InterchangePoint.hh"

namespace hcal {
  namespace uhtr {

    static const char* ConnectionManagerName = "UHAL_CONNECTION_MANAGER";
    using hcal::toolbox::InterchangePoint;


    uhal::ConnectionManager* getConnectionManager() {
      uhal::ConnectionManager* cman;

      if (!InterchangePoint::getInstance().hasObject(ConnectionManagerName) || InterchangePoint::getInstance().getObject(ConnectionManagerName)==0) {
	if (hcalCrate::getInstance()==0) {
	  XCEPT_RAISE(hcal::exception::Exception,"No hcalCrate object, unable to obtain UHAL_CONNECTIONS_FILE object");
	}
	std::string connectionFile=hcalCrate::getInstance()->uhal_connectionsFile();
	return getConnectionManager(connectionFile);
      } else {
	cman=(uhal::ConnectionManager*)(InterchangePoint::getInstance().getObject(ConnectionManagerName));
      }
      return cman;
    }
    
    uhal::ConnectionManager* getConnectionManager(const std::string& connectionFile) {
      uhal::ConnectionManager* cman;
      if (!InterchangePoint::getInstance().hasObject(ConnectionManagerName) || InterchangePoint::getInstance().getObject(ConnectionManagerName)==0) {
	if (connectionFile.empty()) {
	  XCEPT_RAISE(hcal::exception::Exception,"Expected a UHAL_CONNECTIONS_FILE item from the hcalCrate");
	}
	
	try {
	  cman=new uhal::ConnectionManager(connectionFile);
	} RETHROW_UHAL_EXCEPTION;
	InterchangePoint::getInstance().addObject(ConnectionManagerName,cman);
      } else {
	cman=(uhal::ConnectionManager*)(InterchangePoint::getInstance().getObject(ConnectionManagerName));
      }
      return cman;

    }
    void resetConnectionManager() {
      if (InterchangePoint::getInstance().hasObject(ConnectionManagerName)) {
	uhal::ConnectionManager* cman=(uhal::ConnectionManager*)(InterchangePoint::getInstance().getObject(ConnectionManagerName));
	InterchangePoint::getInstance().removeObject(ConnectionManagerName);
	delete cman;
	
      }
    }
  }
}


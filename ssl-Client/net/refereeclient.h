#ifndef REFEREECLIENT_H
#define REFEREECLIENT_H
#include "netraw.h"
#include <string>

#include "pb/vssref_command.pb.h"
#include "pb/vssref_common.pb.h"
#include "pb/vssref_placement.pb.h"

using namespace std;
/**
    @author Author Name
*/

class refereeClient{
protected:
  static const int MaxDataGramSize = 65536;
  char * in_buffer;
  Net::UDP mc; // multicast client
  int _port;
  string _net_address;
  string _net_interface;
public:
    refereeClient(string net_ref_address, int port, string net_ref_interface = "");

    ~refereeClient();
    bool open(bool blocking=false);
    void close();
    bool receive(VSSRef::ref_to_team::VSSRef_Command & packet);

};


#endif // REFEREECLIENT_H

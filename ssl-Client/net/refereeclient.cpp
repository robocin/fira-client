#include "refereeclient.h"

refereeClient::refereeClient(string net_address,
                                    int port,
                                    string net_interface)
{
  _port=port;
  _net_address=net_address;
  _net_interface=net_interface;
  in_buffer=new char[65536];
}


refereeClient::~refereeClient()
{
  delete[] in_buffer;
}

void refereeClient::close() {
  mc.close();
}

bool refereeClient::open(bool blocking) {
  close();
  if(!mc.open(_port,true,true,blocking)) {
    fprintf(stderr,"Unable to open UDP network port: %d\n",_port);
    fflush(stderr);
    return(false);
  }

  Net::Address multiaddr,interface;
  multiaddr.setHost(_net_address.c_str(),_port);
  if(_net_interface.length() > 0){
    interface.setHost(_net_interface.c_str(),_port);
  }else{
    interface.setAny();
  }

  if(!mc.addMulticast(multiaddr,interface)) {
    fprintf(stderr,"Unable to setup UDP multicast\n");
    fflush(stderr);
    return(false);
  }

  return(true);
}

bool refereeClient::receive(VSSRef::ref_to_team::VSSRef_Command & packet) {
  Net::Address src;
  int r=0;
  r = mc.recv(in_buffer,MaxDataGramSize,src);
  if (r>0) {
    fflush(stdout);
    //decode packet:
    return packet.ParseFromArray(in_buffer,r);
  }
  return false;
}

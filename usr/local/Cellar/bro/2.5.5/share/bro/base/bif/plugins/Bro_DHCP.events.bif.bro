# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/dhcp/events.bif (plugin mode).

## Generated for DHCP messages of type *DHCPDISCOVER* (client broadcast to locate
## available servers).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## req_addr: The specific address requested by the client.
##
## host_name: The value of the host name option, if specified by the client.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_decline dhcp_ack dhcp_nak
##              dhcp_release dhcp_inform
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
export {
global dhcp_discover: event(c: connection , msg: dhcp_msg , req_addr: addr , host_name: string );


## Generated for DHCP messages of type *DHCPOFFER* (server to client in response
## to DHCPDISCOVER with offer of configuration parameters).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## mask: The subnet mask specified by the message.
##
## router: The list of routers specified by the message.
##
## lease: The least interval specified by the message.
##
## serv_addr: The server address specified by the message.
##
## host_name: Optional host name value. May differ from the host name requested
##            from the client.
##
## .. bro:see:: dhcp_discover dhcp_request dhcp_decline dhcp_ack dhcp_nak
##              dhcp_release dhcp_inform
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
global dhcp_offer: event(c: connection , msg: dhcp_msg , mask: addr , router: dhcp_router_list , lease: interval , serv_addr: addr , host_name: string );


## Generated for DHCP messages of type *DHCPREQUEST* (Client message to servers either
## (a) requesting offered parameters from one server and implicitly declining offers
## from all others, (b) confirming correctness of previously allocated address after,
## e.g., system reboot, or (c) extending the lease on a particular network address.)
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## req_addr: The client address specified by the message.
##
## serv_addr: The server address specified by the message.
##
## host_name: The value of the host name option, if specified by the client.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_decline dhcp_ack dhcp_nak
##              dhcp_release dhcp_inform
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
global dhcp_request: event(c: connection , msg: dhcp_msg , req_addr: addr , serv_addr: addr , host_name: string );


## Generated for DHCP messages of type *DHCPDECLINE* (Client to server indicating
## network address is already in use).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## host_name: Optional host name value.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_ack dhcp_nak
##              dhcp_release dhcp_inform
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
global dhcp_decline: event(c: connection , msg: dhcp_msg , host_name: string );


## Generated for DHCP messages of type *DHCPACK* (Server to client with configuration
## parameters, including committed network address).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## mask: The subnet mask specified by the message.
##
## router: The list of routers specified by the message.
##
## lease: The least interval specified by the message.
##
## serv_addr: The server address specified by the message.
##
## host_name: Optional host name value. May differ from the host name requested
##            from the client.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_decline dhcp_nak
##              dhcp_release dhcp_inform
##
global dhcp_ack: event(c: connection , msg: dhcp_msg , mask: addr , router: dhcp_router_list , lease: interval , serv_addr: addr , host_name: string );


## Generated for DHCP messages of type *DHCPNAK* (Server to client indicating client's
## notion of network address is incorrect (e.g., client has moved to new subnet) or
## client's lease has expired).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## host_name: Optional host name value.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_decline dhcp_ack dhcp_release
##              dhcp_inform
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
global dhcp_nak: event(c: connection , msg: dhcp_msg , host_name: string );


## Generated for DHCP messages of type *DHCPRELEASE* (Client to server relinquishing
## network address and cancelling remaining lease).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## host_name: The value of the host name option, if specified by the client.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_decline dhcp_ack dhcp_nak
##              dhcp_inform
##
global dhcp_release: event(c: connection , msg: dhcp_msg , host_name: string );


## Generated for DHCP messages of type *DHCPINFORM* (Client to server, asking only for
## local configuration parameters; client already has externally configured network
## address).
##
## c: The connection record describing the underlying UDP flow.
##
## msg: The parsed type-independent part of the DHCP message.
##
## host_name: The value of the host name option, if specified by the client.
##
## .. bro:see:: dhcp_discover dhcp_offer dhcp_request dhcp_decline dhcp_ack dhcp_nak
##              dhcp_release
##
## .. note:: Bro does not support broadcast packets (as used by the DHCP
##    protocol). It treats broadcast addresses just like any other and
##    associates packets into transport-level flows in the same way as usual.
##
global dhcp_inform: event(c: connection , msg: dhcp_msg , host_name: string );


} # end of export section
module GLOBAL;

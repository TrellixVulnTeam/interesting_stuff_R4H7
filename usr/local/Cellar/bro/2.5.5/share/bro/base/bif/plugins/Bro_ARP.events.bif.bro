# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/arp/events.bif (plugin mode).

## Generated for ARP requests.
##
## See `Wikipedia <http://en.wikipedia.org/wiki/Address_Resolution_Protocol>`__
## for more information about the ARP protocol.
##
## mac_src: The request's source MAC address.
##
## mac_dst: The request's destination MAC address.
##
## SPA: The sender protocol address.
##
## SHA: The sender hardware address.
##
## TPA: The target protocol address.
##
## THA: The target hardware address.
##
## .. bro:see:: arp_reply  bad_arp
export {
global arp_request: event(mac_src: string , mac_dst: string , SPA: addr , SHA: string , TPA: addr , THA: string );


## Generated for ARP replies.
##
## See `Wikipedia <http://en.wikipedia.org/wiki/Address_Resolution_Protocol>`__
## for more information about the ARP protocol.
##
## mac_src: The reply's source MAC address.
##
## mac_dst: The reply's destination MAC address.
##
## SPA: The sender protocol address.
##
## SHA: The sender hardware address.
##
## TPA: The target protocol address.
##
## THA: The target hardware address.
##
## .. bro:see::  arp_request bad_arp
global arp_reply: event(mac_src: string , mac_dst: string , SPA: addr , SHA: string , TPA: addr , THA: string );


## Generated for ARP packets that Bro cannot interpret. Examples are packets
## with non-standard hardware address formats or hardware addresses that do not
## match the originator of the packet.
##
## SPA: The sender protocol address.
##
## SHA: The sender hardware address.
##
## TPA: The target protocol address.
##
## THA: The target hardware address.
##
## explanation: A short description of why the ARP packet is considered "bad".
##
## .. bro:see:: arp_reply arp_request
##
## .. todo:: Bro's current default configuration does not activate the protocol
##    analyzer that generates this event; the corresponding script has not yet
##    been ported to Bro 2.x. To still enable this event, one needs to
##    register a port for it or add a DPD payload signature.
global bad_arp: event(SPA: addr , SHA: string , TPA: addr , THA: string , explanation: string );

} # end of export section
module GLOBAL;

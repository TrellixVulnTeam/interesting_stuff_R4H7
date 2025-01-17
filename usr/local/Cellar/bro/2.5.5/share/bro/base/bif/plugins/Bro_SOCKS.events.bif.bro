# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/socks/events.bif (plugin mode).

## Generated when a SOCKS request is analyzed.
##
## c: The parent connection of the proxy.
##
## version: The version of SOCKS this message used.
##
## request_type: The type of the request.
##
## sa: Address that the tunneled traffic should be sent to.
##
## p: The destination port for the proxied traffic.
##
## user: Username given for the SOCKS connection.  This is not yet implemented
##       for SOCKSv5.
export {
global socks_request: event(c: connection , version: count , request_type: count , sa: SOCKS::Address , p: port , user: string );


## Generated when a SOCKS reply is analyzed.
##
## c: The parent connection of the proxy.
##
## version: The version of SOCKS this message used.
##
## reply: The status reply from the server.
##
## sa: The address that the server sent the traffic to.
##
## p: The destination port for the proxied traffic.
global socks_reply: event(c: connection , version: count , reply: count , sa: SOCKS::Address , p: port );


## Generated when a SOCKS client performs username and password based login.
##
## c: The parent connection of the proxy.
##
## user: The given username.
##
## password: The given password.
global socks_login_userpass_request: event(c: connection , user: string , password: string );


## Generated when a SOCKS server replies to a username/password login attempt.
##
## c: The parent connection of the proxy.
## 
## code: The response code for the attempted login.
global socks_login_userpass_reply: event(c: connection , code: count );


} # end of export section
module GLOBAL;

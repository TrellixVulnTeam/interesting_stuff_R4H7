# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/rfb/events.bif (plugin mode).

## Generated for RFB event
##
## c: The connection record for the underlying transport-layer session/flow.
export {
global rfb_event: event(c: connection );


## Generated for RFB event authentication mechanism selection
##
## c: The connection record for the underlying transport-layer session/flow.
##
## authtype: the value of the chosen authentication mechanism
global rfb_authentication_type: event(c: connection , authtype: count );


## Generated for RFB event authentication result message
##
## c: The connection record for the underlying transport-layer session/flow.
##
## result: whether or not authentication was succesful
global rfb_auth_result: event(c: connection , result: bool );


## Generated for RFB event share flag messages
##
## c: The connection record for the underlying transport-layer session/flow.
##
## flag: whether or not the share flag was set
global rfb_share_flag: event(c: connection , flag: bool );


## Generated for RFB event client banner message
##
## c: The connection record for the underlying transport-layer session/flow.
##
## version: of the client's rfb library
global rfb_client_version: event(c: connection , major_version: string , minor_version: string );


## Generated for RFB event server banner message
##
## c: The connection record for the underlying transport-layer session/flow.
##
## version: of the server's rfb library
global rfb_server_version: event(c: connection , major_version: string , minor_version: string );


## Generated for RFB event server parameter message
##
## c: The connection record for the underlying transport-layer session/flow.
##
## name: name of the shared screen
##
## width: width of the shared screen
##
## height: height of the shared screen
global rfb_server_parameters: event(c: connection , name: string , width: count , height: count );
} # end of export section
module GLOBAL;

# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/smb/smb1_com_transaction.bif (plugin mode).

## Generated for :abbr:`SMB (Server Message Block)`/:abbr:`CIFS (Common Internet File System)`
## version 1 requests of type *transaction*. This command serves as the transport for the
## Transaction Subprotocol Commands. These commands operate on mailslots and named pipes,
## which are interprocess communication endpoints within the CIFS file system.
##
## For more information, see MS-CIFS:2.2.4.33
##
## c: The connection.
##
## hdr: The parsed header of the :abbr:`SMB (Server Message Block)` version 1 message.
##
## name: A name string that MAY identify the resource (a specific Mailslot or Named Pipe) 
##       against which the operation is performed.
##
## sub_cmd: The sub command, some may be parsed and have their own events.
##
## .. bro:see:: smb1_message smb1_transaction2_request
export {
global smb1_transaction_request: event(c: connection , hdr: SMB1::Header , name: string , sub_cmd: count );

} # end of export section
module GLOBAL;

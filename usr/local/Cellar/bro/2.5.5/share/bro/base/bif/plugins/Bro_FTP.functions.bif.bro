# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/ftp/functions.bif (plugin mode).


export {




## Converts a string representation of the FTP PORT command to an
## :bro:type:`ftp_port`.
##
## s: The string of the FTP PORT command, e.g., ``"10,0,0,1,4,31"``.
##
## Returns: The FTP PORT, e.g., ``[h=10.0.0.1, p=1055/tcp, valid=T]``.
##
## .. bro:see:: parse_eftp_port parse_ftp_pasv parse_ftp_epsv fmt_ftp_port
global parse_ftp_port: function(s: string ): ftp_port ;


## Converts a string representation of the FTP EPRT command (see :rfc:`2428`)
## to an :bro:type:`ftp_port`.  The format is
## ``"EPRT<space><d><net-prt><d><net-addr><d><tcp-port><d>"``,
## where ``<d>`` is a delimiter in the ASCII range 33-126 (usually ``|``).
##
## s: The string of the FTP EPRT command, e.g., ``"|1|10.0.0.1|1055|"``.
##
## Returns: The FTP PORT, e.g., ``[h=10.0.0.1, p=1055/tcp, valid=T]``.
##
## .. bro:see:: parse_ftp_port parse_ftp_pasv parse_ftp_epsv fmt_ftp_port
global parse_eftp_port: function(s: string ): ftp_port ;


## Converts the result of the FTP PASV command to an :bro:type:`ftp_port`.
##
## str: The string containing the result of the FTP PASV command.
##
## Returns: The FTP PORT, e.g., ``[h=10.0.0.1, p=1055/tcp, valid=T]``.
##
## .. bro:see:: parse_ftp_port parse_eftp_port parse_ftp_epsv fmt_ftp_port
global parse_ftp_pasv: function(str: string ): ftp_port ;


## Converts the result of the FTP EPSV command (see :rfc:`2428`) to an
## :bro:type:`ftp_port`.  The format is ``"<text> (<d><d><d><tcp-port><d>)"``,
## where ``<d>`` is a delimiter in the ASCII range 33-126 (usually ``|``).
##
## str: The string containing the result of the FTP EPSV command.
##
## Returns: The FTP PORT, e.g., ``[h=10.0.0.1, p=1055/tcp, valid=T]``.
##
## .. bro:see:: parse_ftp_port parse_eftp_port parse_ftp_pasv fmt_ftp_port
global parse_ftp_epsv: function(str: string ): ftp_port ;


## Formats an IP address and TCP port as an FTP PORT command. For example,
## ``10.0.0.1`` and ``1055/tcp`` yields ``"10,0,0,1,4,31"``.
##
## a: The IP address.
##
## p: The TCP port.
##
## Returns: The FTP PORT string.
##
## .. bro:see:: parse_ftp_port parse_eftp_port parse_ftp_pasv parse_ftp_epsv
global fmt_ftp_port: function(a: addr , p: port ): string ;


} # end of export section
module GLOBAL;

# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/analyzer/protocol/stepping-stone/events.bif (plugin mode).

## Deprecated. Will be removed.
export {
global stp_create_endp: event(c: connection , e: int , is_orig: bool );




## Event internal to the stepping stone detector.
global stp_resume_endp: event(e: int );


## Event internal to the stepping stone detector.
global stp_correlate_pair: event(e1: int , e2: int );


## Event internal to the stepping stone detector.
global stp_remove_pair: event(e1: int , e2: int );


## Event internal to the stepping stone detector.
global stp_remove_endp: event(e: int );


} # end of export section
module GLOBAL;
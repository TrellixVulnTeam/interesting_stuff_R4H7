# This file was automatically generated by bifcl from /tmp/bro-20180829-84347-dlsyak/bro-2.5.5/src/file_analysis/file_analysis.bif (alternative mode).

##! Internal functions and types used by the file analysis framework.

export {
module Files;






## :bro:see:`Files::set_timeout_interval`.
global Files::__set_timeout_interval: function(file_id: string , t: interval ): bool ;


## :bro:see:`Files::enable_reassembly`.
global Files::__enable_reassembly: function(file_id: string ): bool ;


## :bro:see:`Files::disable_reassembly`.
global Files::__disable_reassembly: function(file_id: string ): bool ;


## :bro:see:`Files::set_reassembly_buffer_size`.
global Files::__set_reassembly_buffer: function(file_id: string , max: count ): bool ;


## :bro:see:`Files::add_analyzer`.
global Files::__add_analyzer: function(file_id: string , tag: Files::Tag , args: any ): bool ;


## :bro:see:`Files::remove_analyzer`.
global Files::__remove_analyzer: function(file_id: string , tag: Files::Tag , args: any ): bool ;


## :bro:see:`Files::stop`.
global Files::__stop: function(file_id: string ): bool ;


## :bro:see:`Files::analyzer_name`.
global Files::__analyzer_name: function(tag: Files::Tag ) : string ;


module GLOBAL;


## For use within a :bro:see:`get_file_handle` handler to set a unique
## identifier to associate with the current input to the file analysis
## framework.  Using an empty string for the handle signifies that the
## input will be ignored/discarded.
##
## handle: A string that uniquely identifies a file.
##
## .. bro:see:: get_file_handle
global set_file_handle: function(handle: string ): any ;



} # end of export section
module GLOBAL;

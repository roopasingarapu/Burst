`BURST(1)` 

## SYNOPSIS 

`burst` \[filename\] 
`burst` \[-l\] \[number\] \[filename\] 
`burst` \[--lines\] \[number\] \[filename\] 

## DESCRIPTION 

The program `burst` allows you to work with the text format. After 
bursting the file, all text from the original source file is preserved 
including white-space, comments, and preprocessor statements. No 
preprocessing of the source code is done. In addition, the tool can be 
applied to individual source files, or code fragments. 

By default, burst splits the file into multiple files each with 500 
lines if the input file has more than 500 lines. 

## GENERAL OPTIONS 

`-h`, `--help` 
: Report bugs to: rs189@zips.uakron.edu burst home page: 
<http://www.gnu.org/software/burst/> General help using GNU software: 
<http://www.gnu.org/gethelp/> 

`-V`, `--version` 
: GNU hello 2.3 Copyright (C) 2007 Free Software 
Foundation, Inc. License GPLv3+: GNU GPL version 3 or later 
<http://gnu.org/licenses/gpl.html> This is free software: you are free 
to change and redistribute it. There is NO WARRANTY, to the extent 
permitted by law. 

`-l`, `--lines` 
: Splits the file into multiple files based on the 
number specified. 

### EXAMPLES 

burst -l 50 abc.txt 
: Creates multiple files each with 50 lines if the 
input file has more than 50 lines. 

burst abc.txt 
: Creates multiple files each with 500 lines if the input 
file has more than 500 lines. 

## RETURN STATUS 

* 0: Normal 
* 1: Error 

## BUGS 

Report bugs to rs189@zips.uakron.edu 

## SEE ALSO 

`split`(1) 

## AUTHORS 

Written by Roopa Singarapu 

## COPYRIGHT 

Copyright (C) 2013-2015 burst, LLC. (www.burst.org) 

The burst is free software; you can redistribute it and/or modify it 
under the terms of the GNU General Public License as published by the 
Free Software Foundation; either version 2 of the License, or (at your 
option) any later version. 

The burst Toolkit is distributed in the hope that it will be useful, but 
WITHOUT ANY WARRANTY; without even the implied warranty of 
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General 
Public License for more details. 

You should have received a copy of the GNU General Public License along 
with the burst Toolkit; if not, write to the Free Software Foundation, 
Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA 



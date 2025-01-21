# QB Tools

WIP QB tools, currently tested on

* THPG
* THAW PC


## Tools

### QDeopt
This tool is used to convert the "symbol dump" files into "token dump" files. Symbol dumps being the newer QB format introduced in THAW, and token dumps being what was used in THUG2 and earlier games.
### QOpt
This tool converts the "token dump" files into "symbol dump" files. Essentially from the older format back into the newer.
### QDump
This tool converts a token dump into a readable format (trying to adhere to the origianl Q language as much as possible) 
### QComp
This tool builds Q files (WIP)
### Pakdir
Creates PAK/PAB files
### Unpak
Unpaks PAK/PAB files

### Pregen
Creates THUG-style PRE files

## Unpre
Extract THUG-style pre files

## Misc
Set the `QBTOOLS_DBGINFO_PATH` to point to the path to store the dbginfo checksum db. Import data using `dbginfo_pakimport` and `dbginfo_preimport` to import names from a DBG.PAK or QB.PRE file


## TODO
* Support structure script symbols
* Better error handling / user friendly errors
* THPS3/4 support (PRE and QB differences)
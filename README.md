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
### Pakdir
Creates PAK/PAB files
### Unpak
Unpaks PAK/PAB files

## Misc
Set the `QBTOOLS_CHECKSUM_PATH` to point to a DBG.PAK if you want to have checksum names injected into the token dump from qdeopt, or file names from unpak.


## TODO
* QComp tool
* Support structure script symbols
* Random tokens jump marker `@` is not displaying properly for some entries
# New in version 3.35

* Fixed a corner case in decoding of associated fields (#52)

# New in version 3.34

* Fixed build on Fedora 36 (#49)

# New in version 3.33

* Fixed decoding of missing values in compressed MODES messages (#48)

# New in version 3.33

* Conversion V <-> mV and NTU <-> LM

# New in version 3.31

* Fixed decoding of associated fields in compressed messages (#47)

# New in version 3.30

* Fixed dtable parser (#43)
* Improved documentation (#42)

# New in version 3.29

* Implemented `var_hook_domain_errors` option, required by dballe:#241

# New in version 3.28

* Implemented `var_clamp_domain_errors` option, required by dballe:#241

# New in version 3.27

* Support Lua 5.4

# New in version 3.26

* Decode compressed string with non-empty reference values (#38)
* Updated wobble and wobblepy
* Always include wreport/version.h so that code that needs it can be compatibile with old versions

# New in version 3.25

* Added NEWS.md (#34)
* Updated lookup/guessing of reference year century on BUFR ed.3 (#36)
* Fix bug with C table modifier 207YYY (#37)
* Improved documentation

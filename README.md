# libaoscdeploykit
The common library for AOSC Deploy Kit

----

## Components

1. Partitions
  - Get partition list
  - Partion formatting
  - (Possible) Partion table editing
  - (Possible) Partion resizing / maintaining
2. Bootstrap/Install
  - Tarball extraction / bootstrap dpkg
  - Package listing
  - Package addition / removal / update / upgrade
3. Repositories
  - Network/Proxy configuration -> Probably just give it to NetworkManager
  - Repository/Mirror selection -> Probably apt-gen-list ?
4. System configuration / maintanence
  - Do we really need this part? Maybe the existing CLI tools are extremely sufficient.

## Dependencies (Proposed)

0. Common
  - TBD

1. Partitions
  - libudev
  - libblkid
  - A kernel running linux
  
2. Bootstrap/Install
  - tar
  - packagekit

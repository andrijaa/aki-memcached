# Aki Memcached

Simple server supporting SET/GET memcached protocol

BUILD/RUN Instructions

AkiServer
1. Requirements 
 - requires boost
 - clang

2. Builds steps 
 - Install boost -> "brew install boost"
 - download source code
 - cd AkiServer
 - run "make"

3. Run
 - "./AkiServer <host> <port> <num-threads>"

AkiClient
1. Requirements
 - python 2.

2. Run
 - "python ./AkiClient.py"

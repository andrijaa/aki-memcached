# Aki Memcache Server

Simple server supporting SET/GET memcached protocol

##AkiServer

###Requirements 
 - requires boost
 - clang

###Builds steps 
 - Install boost -> "brew install boost"
 - download source code
 - cd AkiServer
 - run "make"

###Run
 - "./AkiServer <host> <port> <num-threads>"

##AkiClient
###Requirements
 - python

###Run
 - "python ./AkiClient.py"

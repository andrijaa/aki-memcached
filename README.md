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
 - Command line: AkiServer <host> <port> <num-threads>

 - Example: "./AkiServer"   //runs server with defaults 127.0.0.1:11211 with 8 threads
 - Example: "./AkiServer localhost 13131 16"

##AkiClient
###Requirements
 - python

###Run
 - "python ./AkiClient.py"

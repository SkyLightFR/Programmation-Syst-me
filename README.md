The Tiny Successful Server
=============

TTS Server can now answer to simple HTTP GET requests!

USAGE
-----------

### How to compile
Simply run *make* in the root directory.

### How to run
    $ ./TTS-Server <listen-port>
*listen-port* can be any number between 0 and 65535.

At the moment, the only working feature is the treatment of GET requests. When a HTTP client asks for the */* resource, TTS Server will respond with a valid HTTP header and the content of the file located in resource/welcome_message (this path is currently hard-coded).

NOTICE
-----------
Please don't try to use this program for anything else than testing or learning!

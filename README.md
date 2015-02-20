The Tiny Successful Server
=============

TTS Server can now answer to simple HTTP GET requests!

USAGE
-----------

### How to compile
Simply run *make* in the root directory.

### How to run
    $ ./TTS-Server [-d root-directory] <listen-port>
*listen-port* can be any number between 0 and 65535.
*root-directory* must be a readable directory. The path can be either absolute or relative to the execution directory.

At the moment, the only working feature is the treatment of GET requests. When an HTTP client asks for any existing accessible resource within the filesystem, TTS Server will respond with a valid HTTP status, a Content-Length header and the content of the requested file.

NOTICE
-----------
Please don't try to use this program for anything else than testing or learning!

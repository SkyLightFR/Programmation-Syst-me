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

At the moment, working features are:
- Treatment of GET requests. When an HTTP client asks for an existing accessible resource within the filesystem, TTS Server will respond with a valid HTTP status, a header composed of a Content-Length and a Content-Type, and the content of the requested file ;
- Stats of the status of previous requests and responses. You can view them in an HTML-formatted table at *<yourserver>*/stats.

NOTICE
-----------
Please don't try to use this program for anything else than testing or learning!

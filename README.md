wirefang
========

Overview
--------

Packet injection utility.

Wirefang reads bytes from a file and then writes those bytes to the data link 
layer (layer 2) of a network interface.

Compatibility
-------------

* Linux

Installation
------------

Full installation of wirefang is a 2 step process. However, the second step is 
optional.

Firstly, navigate to the src directory and run the following command:

    $ bash build.sh

Once this has completed, you can run wirefang by navigating to the bin 
directory and running the following command:

    # ./wirefang

Secondly, and optionally, you can install wirefang system-wide by copying the 
contents of the bin directory into your path. For example, you could execute a 
command such as:

    # cp bin/* /usr/local/bin/

License
-------

Copyright (c) 2014 Damian Jason Lapidge

Licensing terms and conditions can be found within the file LICENSE.txt.


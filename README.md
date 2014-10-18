WiControl
=========

Wireless Control with Panstamp and Raspberry Pi

The libserial-0.6.0rc1 directory contains a version of the C++ libserial
library configured for the Raspberry Pi.

Installation:
-------------

On the Pi:

$ mkdir Panstamp
$ cd Panstamp
$ git clone https://github.com/AdrianRepRap/WiControl.git
$ cd WiControl/libserial-0.6.0rc1
$ make clean
$ sudo make install
$ cd ../Host-software/Debug
$ make clean
$ make
$ cd ../..
$ cp run-heat-control.sample run-heat-control

then edit run-heat-control to download your control file from your webserver and to 
run the program with that file.  Finally

$ crontab -e

and add a line at the end like this:

* * * * * /home/pi/Panstamp/WiControl/run-heat-control

That will cause the shell script run-heat-control to be run once a minute.

When it runs it creates a status file called Status.

$ cat Status

will tell you what your system last did.  If you

$ touch Log

then each time run-heat-control runs it will append the status
report to the end of itself, which keeps a history and is therefore
useful for debugging.

When you have finished debugging don't forget to

$ rm Log

otherwise it will eventually fill up the SD/disc...


Adrian Bowyer


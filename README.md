Minimalist serial console
=========================
This is a console that lets you manage your serial devices through AT commands.
Although the target device of this program are Wavecom GSM devices, so it may
need a few changes to work properly with other devices.

The goal of this program is to provide a minimal (humble) testing environment,
and as a reference for building larger projects as well.

Tested with a Wavecom Fastrack M1206B

Compile
-------
    make 

KNOWN BUGS
----------
This program processes the dots (.) as CTRL-Z, so it may be a little difficult
to send a SMS with dots in it.

License
-------
This content is released under the MIT License. For more information
refer to file COPYRIGHT included in this distribution.

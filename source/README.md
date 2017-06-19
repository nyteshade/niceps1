# getcip.c
This is the current state of the getcip command line utility used to quickly read the network cards and their associated ip addresses. Without this application, the niceps1 script attempts to use python or node js to read the ip address but these are understandably slower than a native command to do so.

This source code is here for viewing purposes only. It is already gzipped and base64 encoded into the niceps1 script. The script extracts the source contents and builds if possible for you when `niceps1 buildCIP` is invoked. 

`NOTE` the code is intentionally sparse of comments given its contents are embedded in the shell script

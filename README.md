sk-piano
========

Arduino code for controlling piano and lights

Simulator Setup
---------------

In order to run the simulator you need a few things set up:
* ruby + ruby gems
* ruby gem: eventmachine (install: sudo gem install eventmachine)
* ruby gem: em-websocket (install: sudo gem install em-websocket)
* gcc/g++ (on mac, this means xcode + command line tools)

To run it, run simulator/websockets_server.rb from the simulator directory, then
browse to simulator/sim.html in Chrome.  (You probably need Chrome, and a recent
version, due to the simulator's use of web sockets)  Every time you reload the
page it will re-run beaglebone/piano (and should kill that process when you
browse away from the page).

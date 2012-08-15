from mod_pywebsocket import msgutil
from datetime import datetime
import time
import json
import subprocess

def web_socket_do_extra_handshake(request):
   print 'Connected.'
   pass  # Always accept.


# TODO figure out how to kill the process when the connection dies
# TODO keyboard input
def web_socket_transfer_data(request):
  process = subprocess.Popen(["../beaglebone/piano"], stdout=subprocess.PIPE)
  while True:
    line = process.stdout.readline()
    if line == "":
      return
    msgutil.send_message(request, line)

#    date = datetime.now()
#    try:
#        line = msgutil.receive_message(request)
#    except Exception, e:
#        print 'Foi com os porcos'
#        raise e
#    print 'Got something: %s' % line
#msgutil.send_message(request, line)
#    msgutil.send_message(request, 'clock!%s' % date)
    #if line == _GOODBYE_MESSAGE:
    #    return


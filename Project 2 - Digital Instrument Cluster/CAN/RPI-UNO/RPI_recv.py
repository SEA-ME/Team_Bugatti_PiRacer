import can

can_interface = 'can1'
bus = can.interface.Bus(can_interface, bustype='socketcan')
while True:
    message = bus.recv(1.0) # Timeout in seconds.
    if message is None:
        print('Timeout occured, no message.')
    print(message)

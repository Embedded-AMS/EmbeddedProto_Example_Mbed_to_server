import socket
import signal
import sys
from threading import Thread
from generated import eth_messages_pb2
 
if __name__ == "__main__":

    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_address = ('', 50000)
    sock.bind(server_address)

    print("Hostname:", socket.gethostname(), "IP address:", socket.gethostbyname(socket.gethostname()))

    # Listen for incoming connections
    sock.listen(1)
    
    while True:
        # Wait for a connection
        print('waiting for a connection')
        connection, client_address = sock.accept()
        print('connection from', client_address)
        
        
        try:      
            # Receive the data in small chunks and retransmit it
            while True:
                data = connection.recv(50)
                
                length = data[0]
                
                if 0 < length:
                                             
                    # Check if we have received all bytes.
                    if length == len(data[1:]):
                        data = data[1:length+1]
                        status = eth_messages_pb2.Status()
                        status.ParseFromString(data)
                    
                        print("\nStatus Message")
                        if status.led == eth_messages_pb2.Status.LED.LedUndefined:
                            print("Led status: undefined")
                        elif status.led == eth_messages_pb2.Status.LED.LedOff:
                            print("Led status: off")
                        elif status.led == eth_messages_pb2.Status.LED.LedOn:
                            print("Led status: on")   
                            
                        print("Run time: ", "{:.2f}".format(status.run_time/1e6), "seconds")
                        
                        print("Temperature: ", "{:.2f}".format(status.temperature), "degrees celcius")
                        print("Humidity: ", "{:.2f}%".format(status.humidity))
                        print("Windspeed: ", "{:.2f}km/h".format(status.windspeed))
        
        except KeyboardInterrupt:
            print("Exit program")
            sock.close()
            sys.exit(0)
                
        finally:
            # Clean up the connection
            connection.close()
 

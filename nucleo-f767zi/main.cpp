#include "mbed.h"
#include "mbed_events.h"
#include "EthernetInterface.h"
#include "eth_messages.h"
#include <ReadBufferFixedSize.h>
#include <WriteBufferFixedSize.h>
#include <Errors.h>
#include <cstdint>


EthernetInterface eth;
TCPSocket socket;

InterruptIn button(USER_BUTTON);
DigitalOut led2(LED2);
DigitalOut led3(LED3);

constexpr uint32_t BUFFER_SIZE = 50;
::EmbeddedProto::ReadBufferFixedSize<BUFFER_SIZE> read_buffer;
::EmbeddedProto::WriteBufferFixedSize<BUFFER_SIZE> write_buffer;
Status status;

EventQueue queue;
Timer t;


void disconnect()
{
    // Close the socket to return its memory and bring down the network interface
    socket.close();

    // Bring down the ethernet interface
    eth.disconnect();
    led3.write(1);
    printf("Done\n");
    exit(-2);
}

void send_status()
{
    nsapi_size_or_error_t result;
    uint64_t run_time =  t.elapsed_time().count();

    switch(status.led()){
        case Status::LED::LedUndefined:
            led2.write(1);
            status.set_led(Status::LED::LedOn);
            break;
        case Status::LED::LedOff:
            led2.write(1);
            status.set_led(Status::LED::LedOn);
            break;
        case Status::LED::LedOn:
            led2.write(0);
            status.set_led(Status::LED::LedOff);
            break;
        default:
            break;
    } 

    status.set_run_time(run_time);
    
    float temperature = 5.0 * ((float)rand() / (float) RAND_MAX) + 20.0; //random number between 20.0 and 25.0
    float humidity = 30.0 * ((float)rand() / (float) RAND_MAX) + 50.0; //random number between 50.0 and 80.0
    float windspeed = 20.0 * ((float)rand() / (float) RAND_MAX); //random number between 0 and 20.0

    status.set_temperature(temperature);
    status.set_humidity(humidity);
    status.set_windspeed(windspeed);

    uint32_t message_len_index = write_buffer.get_size();
	write_buffer.push(0); //placeholder for message length

    auto serialization_status = status.serialize(write_buffer);
	if(::EmbeddedProto::Error::NO_ERRORS == serialization_status)
	{
        write_buffer.get_data()[message_len_index] = write_buffer.get_size()-1;
        result = socket.send(write_buffer.get_data(), (nsapi_size_t) write_buffer.get_size());
            if (result < 0) {
                printf("Error! socket.send() returned: %d\n", result);
                disconnect();
            }
            else{
                printf("Status sent\n");
            }
        
	}

    write_buffer.clear();    
}

void user_button(void)
{
    queue.break_dispatch(); //prevent button debouncing 
    send_status();  
    wait_us(120000);
    queue.dispatch_forever();
}

int main() {
    int remaining;
    int rcount;
    char *p;
    char buffer[256];
    nsapi_size_or_error_t result;
    bool valid;
    uint32_t i=0;

    //start runtime counter
    t.start();

   // create a thread that'll run the event queue's dispatch function
    Thread eventThread;
    eventThread.start(callback(&queue, &EventQueue::dispatch_forever));

    // Bring up the ethernet interface
    printf("Embedded Proto socket example using Mbed OS\n");

#ifdef MBED_MAJOR_VERSION
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);
#endif


    eth.set_dhcp(true);
    result = eth.connect();

    if (result != 0) {
        printf("Error! net->connect() returned: %d\n", result);
        return result;
    }

    // Show the network address
    SocketAddress a;
    eth.get_ip_address(&a);
    printf("IP address: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    eth.get_netmask(&a);
    printf("Netmask: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");
    eth.get_gateway(&a);
    printf("Gateway: %s\n", a.get_ip_address() ? a.get_ip_address() : "None");

    // Open a socket on the network interface, and create a TCP connection to server
    result = socket.open(&eth);
    if (result != 0) {
        printf("Error! socket.open() returned: %d\n", result);
        disconnect();
    }


    a.set_port(50000);
    // Substitute the IP address of your server here.
    valid =  a.set_ip_address("192.168.0.101");
    
    printf("Connect to: %s\n", (a.get_ip_address() ? a.get_ip_address() : "None") );
    if(!valid){
        printf("Error! ip address not valid\n");
        disconnect();
    }


    result = socket.connect(a);
    if (result != 0) {
        printf("Error! socket.connect() returned: %d\n", result);
        disconnect();
    }


    button.rise(queue.event(callback(user_button)));

    while(1)
    {
        ThisThread::sleep_for(1s);
    }

}

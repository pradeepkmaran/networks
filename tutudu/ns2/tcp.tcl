# Create a simulator instance
set ns [new Simulator]

# Create trace files
set tf [open out.tr w]
$ns trace-all $tf
set nf [open out.nam w]
$ns namtrace-all $nf

# Define the network topology
set n0 [$ns node]  # Source node
set n1 [$ns node]  # Destination node
set n2 [$ns node]  # Intermediate router node

# Create link between source and router, and router to destination
$ns duplex-link $n0 $n2 1Mb 20ms DropTail
$ns duplex-link $n2 $n1 1Mb 20ms DropTail

# Set up FTP traffic generator (CBR can be used as an alternative)
set ftp0 [new Application/FTP]

# Function to configure a TCP agent, attach it to the source and destination, and start the FTP application
proc setup_tcp_simulation {tcp_type} {
    global ns n0 n1 n2 ftp0
    
    # Set up TCP agent with the given type
    set tcp0 [new Agent/$tcp_type]
    $ns attach-agent $n0 $tcp0
    set sink0 [new Agent/TCPSink]
    $ns attach-agent $n1 $sink0
    $ns connect $tcp0 $sink0
    
    # Attach FTP to the TCP agent
    $ftp0 attach-agent $tcp0
    
    # Start and stop the FTP at the given times
    $ns at 0.0 "$ftp0 start"
    $ns at 10.0 "$ftp0 stop"
}

# Function to finish the simulation
proc finish {} {
    global ns nf tf
    $ns flush-trace
    close $nf
    close $tf
    exec nam out.nam &
    exit 0
}

# Simulate for TCP Tahoe
set tcp_type "TCP/Tahoe"
setup_tcp_simulation $tcp_type

# Simulate for TCP Reno
set tcp_type "TCP/Reno"
setup_tcp_simulation $tcp_type

# Simulate for TCP NewReno
set tcp_type "TCP/Newreno"
setup_tcp_simulation $tcp_type

# Simulate for TCP Sack
set tcp_type "TCP/Sack1"
setup_tcp_simulation $tcp_type

# Schedule the finish procedure at time 15.0
$ns at 15.0 "finish"

# Run the simulation
$ns run

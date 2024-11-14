# Create a simulator object
set ns [new Simulator]

# Define different colors for data flows (for NAM)
$ns color 1 Blue
$ns color 2 Red
$ns color 3 Yellow
$ns color 4 Green

# Open the NAM trace
set nf [open out1.nam w]
$ns namtrace-all $nf
set tf [open basic1.tr w]
$ns trace-all $tf

# Define a 'finish' procedure
proc finish {} {
 global ns nf tf
 $ns flush-trace
 # Close the NAM trace
 close $nf
 close $tf
 # Execute NAM on the trace
 exec nam out1.nam &
 exec awk -f Throughput.awk basic1.tr &
 exit 0
}

# Create six nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

# Create links between the nodes
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail
$ns duplex-link $n2 $n4 1.7Mb 20ms DropTail
$ns duplex-link $n2 $n5 2Mb 20ms DropTail
# Set Queue Size of link (n2-n3) to 10
$ns queue-limit $n2 $n3 10

# Give node position (for NAM)
$ns duplex-link-op $n0 $n2 orient right-down
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link-op $n2 $n3 orient right
$ns duplex-link-op $n2 $n4 orient right-down
$ns duplex-link-op $n2 $n5 orient right-up

# Monitor the queue for link (n2-n3). (for NAM)
$ns duplex-link-op $n2 $n3 queuePos 0.5

# Setup a TCP connection (FTP) from n0 to n3
set tcp [new Agent/TCP]
$tcp set class_ 2
$ns attach-agent $n0 $tcp
set sink [new Agent/TCPSink]
$ns attach-agent $n3 $sink
$ns connect $tcp $sink
$tcp set fid_ 1

# Setup a FTP application over TCP
set ftp [new Application/FTP]
$ftp attach-agent $tcp

# Setup a TCP connection (FTP) from n5 to n4
set tcp1 [new Agent/TCP]
$tcp1 set class_ 2
$ns attach-agent $n5 $tcp1
set sink1 [new Agent/TCPSink]
$ns attach-agent $n4 $sink1
$ns connect $tcp1 $sink1
$tcp1 set fid_ 2 ;# Different flow ID for the second TCP connection

# Setup a FTP application over TCP
set ftp1 [new Application/FTP]
$ftp1 attach-agent $tcp1

# Setup a UDP connection from n1 to n3
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp
set null [new Agent/Null]
$ns attach-agent $n3 $null
$ns connect $udp $null
$udp set fid_ 3

# Setup a CBR over UDP connection
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp
$cbr set packet_size_ 1000
$cbr set rate_ 1mb
$cbr set random_ false
# Setup a UDP connection from n0 to n4

set udp1 [new Agent/UDP]
$ns attach-agent $n0 $udp1
set null1 [new Agent/Null]
$ns attach-agent $n4 $null1
$ns connect $udp1 $null1
$udp1 set fid_ 4 ;# Different flow ID for the second UDP connection

# Setup a CBR over UDP connection
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packet_size_ 1000
$cbr1 set rate_ 1mb
$cbr1 set random_ false

# Schedule events for the CBR and FTP agents
$ns at 0.1 "$cbr start"
$ns at 1.0 "$ftp start"
$ns at 4.0 "$ftp stop"
$ns at 4.5 "$cbr stop"
$ns at 5.0 "$cbr1 start"
$ns at 5.5 "$ftp1 start"
$ns at 6.0 "$ftp1 stop"
$ns at 6.5 "$cbr1 stop"

# Call the finish procedure after 7 seconds of simulation time
$ns at 7.0 "finish"

# Print CBR packet size and interval
puts "CBR packet size = [$cbr set packet_size_]"
puts "CBR rate = [$cbr set rate_]"

# Run the simulation
$ns run
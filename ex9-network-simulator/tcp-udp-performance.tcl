# Define the network simulator
set ns [new Simulator]

# Open the trace file
set tracefile [open tcp_udp_performance.tr w]
$ns trace-all $tracefile

# Open the NAM file for animation
set namfile [open tcp_udp_performance.nam w]
$ns namtrace-all $namfile

# Define colors for TCP and UDP
$ns color 1 Red  ;# Color for TCP
$ns color 2 Blue ;# Color for UDP

# Create nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

# Define links between nodes
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns simplex-link $n2 $n3 0.3Mb 100ms DropTail ;# bottleneck link
$ns simplex-link $n3 $n2 0.3Mb 100ms DropTail ;# reverse bottleneck link
$ns duplex-link $n3 $n4 0.5Mb 40ms DropTail
$ns duplex-link $n3 $n5 0.5Mb 40ms DropTail

# Set queue size of the bottleneck link
$ns queue-limit $n2 $n3 10

# Create TCP connection (n0 to n4)
set tcp [new Agent/TCP]
$tcp set window_ 32          ;# Window size
$tcp set packetSize_ 1000    ;# Packet size
$ns attach-agent $n0 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $n4 $sink
$ns connect $tcp $sink

# Set a flow ID for TCP
$tcp set fid_ 1

# Create UDP connection (n1 to n5)
set udp [new Agent/UDP]
$udp set packetSize_ 1000    ;# Packet size
$ns attach-agent $n1 $udp

set null [new Agent/Null]
$ns attach-agent $n5 $null
$ns connect $udp $null

# Set a flow ID for UDP
$udp set fid_ 2

# Set up a CBR traffic generator over UDP
set cbr [new Application/Traffic/CBR]
$cbr set packetSize_ 1000
$cbr set interval_ 0.005
$cbr attach-agent $udp

# Add TCP traffic generator
set tcp_cbr [new Application/Traffic/CBR]
$tcp_cbr set packetSize_ 1000
$tcp_cbr set interval_ 0.01
$tcp_cbr attach-agent $tcp

# Run the simulation
$ns at 0.1 "$cbr start"
$ns at 0.2 "$tcp_cbr start"
$ns at 10.0 "finish"

# Define a finish procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec nam tcp_udp_performance.nam &
    exit 0
}

# Run the simulation for 10 seconds
$ns run

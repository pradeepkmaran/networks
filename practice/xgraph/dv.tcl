set ns [new Simulator]
$ns color 1 Blue
$ns color 2 Red
set nf [open out.nam w]  
set tf [open trace.tr w]    
$ns namtrace-all $nf
$ns trace-all $tf

proc finish {} {
    global ns nf tf
    $ns flush-trace
    close $nf
    close $tf
    exec nam out.nam &
    exit 0
}

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]
set n9 [$ns node]
set n10 [$ns node]
set n11 [$ns node]

$ns duplex-link $n0 $n8 1Mb 10ms DropTail
$ns duplex-link $n1 $n10 1Mb 10ms DropTail
$ns duplex-link $n0 $n9 1Mb 10ms DropTail
$ns duplex-link $n9 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n11 1Mb 10ms DropTail
$ns duplex-link $n11 $n5 1Mb 1000ms DropTail
$ns duplex-link $n7 $n6 1Mb 10ms DropTail
$ns duplex-link $n2 $n3 1Mb 10ms DropTail
$ns duplex-link $n3 $n4 1Mb 10ms DropTail
$ns duplex-link $n4 $n11 1Mb 10ms DropTail
$ns duplex-link $n10 $n7 1Mb 10ms DropTail
$ns duplex-link $n6 $n0 1Mb 10ms DropTail

set udp [new Agent/UDP]
$ns attach-agent $n0 $udp

set null [new Agent/Null]
$ns attach-agent $n5 $null

$ns connect $udp $null
$udp set fid_ 1
$ns color $udp Blue

set udp1 [new Agent/UDP]
$ns attach-agent $n1 $udp1

set null1 [new Agent/Null]
$ns attach-agent $n5 $null1

$ns connect $udp1 $null1
$udp1 set fid_ 2
$ns color $udp1 Red

set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp  ;# Attach CBR to the first UDP agent
$cbr set packetSize_ 1000
$cbr set interval 0.1
$cbr set rate_ 1Mb
$cbr set random_ false

set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1 ;# Attach CBR1 to the second UDP agent
$cbr1 set packetSize_ 1000
$cbr1 set interval 0.1
$cbr1 set rate_ 1Mb
$cbr1 set random_ false

$ns at 0.5 "$cbr start"
$ns at 0.6 "$cbr1 start"
$ns at 5.0 "finish"

$ns rtproto DV
$ns rtmodel-at 3.5 down $n11 $n5 ;# Bringing down the link 11-5
$ns rtmodel-at 4.5 up $n11 $n5 ;# Bringing up the link 11-5 after 1 second
$ns rtmodel-at 3.0 down $n7 $n6 ;# Bringing down the link 7-6            
$ns rtmodel-at 4.0 up $n7 $n6 ;# Bringing up the link 7-6 after 1 second

puts "CBR packet size = [$cbr set packetSize_]"
puts "CBR interval = [$cbr set interval_]"

$ns run
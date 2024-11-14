set ns [new Simulator]

$ns color 1 Blue
$ns color 2 Red

set nf [open out.nam w]
$ns namtrace-all $nf

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
$ns duplex-link $n11 $n5 1Mb 10ms DropTail
$ns duplex-link $n8 $n5 1Mb 10ms DropTail

$ns duplex-link-op $n0 $n8 orient right
$ns duplex-link-op $n8 $n5 orient right
$ns duplex-link-op $n0 $n9 orient right-down
$ns duplex-link-op $n9 $n11 orient right
$ns duplex-link-op $n11 $n5 orient right-up
$ns duplex-link-op $n10 $n11 orient up
$ns duplex-link-op $n1 $n10 orient up

$n2 set X_ 0
$n2 set Y_ 50
$n2 set Z_ 0


$n3 set X_ 50
$n3 set Y_ 50
$n3 set Z_ 0

$n6 set X_ 100
$n6 set Y_ 50
$n6 set Z_ 0

$n7 set X_ 150
$n7 set Y_ 50
$n7 set Z_ 0


$n4 set X_ -50
$n4 set Y_ 50
$n4 set Z_ 0




set udp0 [new Agent/UDP]
set udp1 [new Agent/UDP]
set null0 [new Agent/Null]
set null1 [new Agent/Null]
$ns attach-agent $n0 $udp0
$ns attach-agent $n1 $udp1
$ns attach-agent $n5 $null0
$ns attach-agent $n5 $null1
$ns connect $udp0 $null0
$ns connect $udp1 $null1
$udp0 set fid_ 1
$udp1 set fid_ 2

set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set type_ CBR
$cbr0 set packet_size_ 1000
$cbr0 set rate_ 200kb
$cbr0 set random_ 1
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set type_ CBR
$cbr1 set packet_size_ 1000
$cbr1 set rate_ 200kb
$cbr1 set random_ 1


$ns rtproto LS

$ns rtmodel-at 1.0 down $n11 $n5
$ns rtmodel-at 2.0 up $n11 $n5





$ns at 0.1 "$cbr0 start"
$ns at 4.5 "$cbr0 stop"
$ns at 0.1 "$cbr1 start"
$ns at 4.5 "$cbr1 stop"
$ns at 5.0 "finish"

proc finish {} {
    global ns nf
    $ns flush-trace
    close $nf
    exec nam out.nam &
    exit 0
}

$ns run

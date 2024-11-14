    # Create a simulator object
    set ns [new Simulator]

    # Open the NAM file and the trace file
    set nf [open basic1.nam w]
    $ns namtrace-all $nf
    set tf [open basic1.tr w]
    $ns trace-all $tf

    # Define a 'finish' procedure
    proc finish {} {
        global ns nf tf
        $ns flush-trace
        close $nf
        close $tf
        exec nam basic1.nam &
        exec xgraph reno.xg &
        exit 0
    }

    # Create the network nodes
    set n0 [$ns node]
    set n1 [$ns node]
    set n2 [$ns node]

    # Create duplex links
    $ns duplex-link $n0 $n1 10Mb 10ms DropTail
    $ns duplex-link $n1 $n2 800Kb 50ms DropTail

    # Set queue limit for the router
    $ns queue-limit $n1 $n2 7

    # Visual hints for NAM
    $ns color 0 Red
    $ns duplex-link-op $n0 $n1 orient right
    $ns duplex-link-op $n1 $n2 orient right
    $ns duplex-link-op $n1 $n2 queuePos 0.5

    # Create and configure TCP sending agent
    set tcp [new Agent/TCP/Sack1]
    $tcp set class_ 0
    $tcp set window_ 100
    $tcp set packetSize_ 960
    $ns attach-agent $n0 $tcp

    # Create and attach TCP receive agent (sink)
    set sink [new Agent/TCPSink]
    $ns attach-agent $n2 $sink
    $ns connect $tcp $sink

    # Schedule the data flow
    set ftp [new Application/FTP]
    $ftp attach-agent $tcp
    $ns at 0.0 "$ftp start"
    $ns at 10.0 "finish"

    # Procedure to plot the congestion window
    proc plotWindow {tcpSource outfile} {
        global ns
        set now [$ns now]
        set cwnd [$tcpSource set cwnd_]
        puts $outfile "$now $cwnd"
        $ns at [expr $now + 0.1] "plotWindow $tcpSource $outfile"
    }

    # Open file to log congestion window
    set outfile [open reno.xg w]
    $ns at 0.0 "plotWindow $tcp $outfile"

    # Run the simulation
    $ns run

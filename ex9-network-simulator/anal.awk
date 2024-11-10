# AWK script to parse the NS2 trace file and calculate throughput and packet loss
BEGIN {
    tcp_received = 0; udp_received = 0;
    tcp_dropped = 0; udp_dropped = 0;
    tcp_total_size = 0; udp_total_size = 0;
    simulation_time = 5; # Adjust to your simulation duration in seconds
}

{
    # Check if the event is a packet received ('r') or dropped ('d')
    # Check if the packet type is TCP or UDP (CBR traffic indicates UDP)
    if ($1 == "r" && $5 == "tcp") {
        tcp_received++;
        tcp_total_size += $5; # Field for packet size
    } else if ($1 == "r" && $5 == "cbr") { # CBR typically maps to UDP traffic
        udp_received++;
        udp_total_size += $5;   
    }

    if ($1 == "d" && $5 == "tcp") {
        tcp_dropped++;
    } else if ($1 == "d" && $5 == "cbr") {
        udp_dropped++;
    }
}

END {
    # Calculate throughput in bits per second (bps)
    tcp_throughput = (tcp_total_size * 8) / simulation_time;
    udp_throughput = (udp_total_size * 8) / simulation_time;

    # Print results
    # print "TCP Total size: " tcp_total_size;
    # print "UDP Total size: " udp_total_size;
    print "TCP Throughput: " tcp_throughput " bps";
    print "UDP Throughput: " udp_throughput " bps";
    print "TCP Packets Received: " tcp_received;
    print "UDP Packets Received: " udp_received;
    print "TCP Packet Loss: " tcp_dropped;
    print "UDP Packet Loss: " udp_dropped;
}

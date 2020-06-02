\d .udp

k_send:.Q.dd[`:.;(.z.o;`udp)] 2:(`k_send;3);
k_listen:.Q.dd[`:.;(.z.o;`udp)] 2:(`k_listen;1);

debug:1b;

Send:{[host;port;payload]
  k_send[host;"i"$port;"x"$payload]
  };

Listen:{[port]
  k_listen "i"$port
  };

recv:{[remote_payload]
  if[debug;
    .udp.lr:remote_payload 0;
    .udp.lp:remote_payload 1
    ];
  Recv . remote_payload
  };

\d .

.udp.Recv:{[remote;payload]
  0N!" "sv ("Received";string count payload;"bytes from";remote);
  payload
  };

\
q).udp.Listen 8000
1b
q).udp.Send["localhost";8000;"Hello, World!"]
13i
q)"Received 13 bytes from 127.0.0.1"

q).udp.lp
0x48656c6c6f2c20576f726c6421
q).udp.lr
"127.0.0.1"

q).udp.Send["localhost";8000] -8!{ x + 2 * y + z }
33i
q)"Received 33 bytes from 127.0.0.1"

q).udp.lp
0x010000002100000064000a00110000007b2078202b2032202a2079202b207a207d
q)-9!.udp.lp
{ x + 2 * y + z }

\d .tcp

k_send:.Q.dd[`:.;(.z.o;`tcp)] 2:(`k_send;1);
k_connect:.Q.dd[`:.;(.z.o;`tcp)] 2:(`k_connect;2);
fd:0i;

debug:1b;

Send:{[payload]
  if[not fd > 0;
    '"handle"
    ];
  k_send "x"$payload
  };

Connect:{[host;port]
  if[not fd;
    :.tcp.fd:k_connect[host;"i"$port]
    ];
  '"connected"
  };

recv:{[payload]
  if[debug;
    .tcp.lp:payload
    ];
  Recv payload
  };

disconnect:{[handle]
  .tcp.fd:0i;
  Disconnect handle
  };

\d .

.tcp.Recv:{[payload]
  0N!" "sv ("Received";string count payload;"bytes");
  payload
  };

.tcp.Disconnect:{[handle]
  0N!" "sv (".tcp.Disconnect called";string handle);
  handle
  };

\

q).tcp.Connect["localhost";12345]
3i
q).tcp.Send "Hello, World!"
13
q)"Received 14 bytes"

q).tcp.lp
0x2248656c6c6f2c204b444221220a
q)"c"$.tcp.lp
"\"Hello, KDB!\"\n"

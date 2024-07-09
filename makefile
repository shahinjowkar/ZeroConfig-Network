make: app service relay

app: app.c zcs.c multicast.c
	gcc app.c zcs.c multicast.c -o app -I .

service: service.c zcs.c multicast.c
	gcc service.c zcs.c multicast.c -o service -I .

relay: relay.c multicast.c
	gcc relay.c multicast.c -o relay -I .


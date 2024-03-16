import espnow

e = espnow.ESPNow()
peer = espnow.Peer(mac=b'@\xcaD\xb8d') # the microphone ESP mac adress
e.peers.append(peer)

e.send("Starting...")
for i in range(10):
    e.send(str(i)*20)
e.send(b'end')

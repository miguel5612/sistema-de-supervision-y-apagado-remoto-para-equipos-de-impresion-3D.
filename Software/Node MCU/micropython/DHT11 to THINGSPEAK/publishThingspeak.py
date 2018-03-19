import usocket as _socket
import ussl as ssl
import dht
import machine
import time
API_KEY = 'D9EBWUZRNK5VQRPS' 
DHT_PIN = 16
HOST = 'api.thingspeak.com'
d = dht.DHT11(machine.Pin(DHT_PIN))

def calculateAndPublish():
        try:
            d.measure()
            temperature = d.temperature() - 3
            data = b"api_key="+ API_KEY + "&field1=" + str(temperature) + "&field2=" + str(d.humidity())
            s = _socket.socket()
            ai = _socket.getaddrinfo(HOST, 443)
            addr = ai[0][-1]
            s.connect(addr)
            s = ssl.wrap_socket(s)  
            s.write("POST /update HTTP/1.0\r\n")
            s.write("Host: " + HOST + "\r\n")
            s.write("Content-Length: " + str(len(data)) + "\r\n\r\n")
            s.write(data)
            s.close()
        except:
            import wifimgr
            print("No se pudo imprimir")
            wifimgr.get_connection()


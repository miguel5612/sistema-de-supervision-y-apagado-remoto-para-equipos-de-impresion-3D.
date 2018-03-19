import usocket as _socket
import ussl as ssl
import dht
import machine
import time
def calculateAndPublish():
    try:
        Measure()
    except:
        print('No se pudo medir')
def Measure():
	DHT_PIN = 16
	d = dht.DHT11(machine.Pin(DHT_PIN))
	d.measure()
	temperatureOld = d.temperature() - 3
	time.sleep(2)
	d.measure()
	temperature = d.temperature() - 3
	diferencia = abs(temperatureOld-temperature)
	temperatureOld = temperature
	humidity = d.humidity()
	if diferencia<4 or diferencia>10:
		try:
			publish(temperature,humidity)
		except:
			print("Cant publish this measure")
			boardReset.confirmandreset()
	else:
		print("Ocurrio un error, la diferencia entre temperaturas es mayor a 4")
def publish(temp,hum):
	API_KEY = 'D9EBWUZRNK5VQRPS' 
	HOST = 'api.thingspeak.com'
	data = b"api_key="+ API_KEY + "&field1=" + str(temp) + "&field2=" + str(hum)
	s=_socket.socket()
	ai = _socket.getaddrinfo(HOST, 443)
	addr = ai[0][-1]
	s.connect(addr)
	s = ssl.wrap_socket(s)  
	s.write("POST /update HTTP/1.0\r\n")
	s.write("Host: " + HOST + "\r\n")
	s.write("Content-Length: " + str(len(data)) + "\r\n\r\n")
	s.write(data)
	s.close()


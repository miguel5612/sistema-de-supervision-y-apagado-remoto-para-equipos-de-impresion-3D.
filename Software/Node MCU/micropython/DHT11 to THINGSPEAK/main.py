import wifimgr
import publishThingspeak
import time
print("main invocado")
time.sleep(10)
print("main en ejecucion.")
try:
	while(True):
		print("while true")
		wlan = wifimgr.get_connection()

		if wlan is None:

			print("Could not initialize the network connection.")

		try:
			print("Tomando muestra y publicando")
			publishThingspeak.calculateAndPublish()
			print("publicado")
			time.sleep(10)
			print("paso el tiempo.")

		except:
			wlan = wifimgr.get_connection()
except:
	print("Fallo el while True, Reintentando...")
	main()
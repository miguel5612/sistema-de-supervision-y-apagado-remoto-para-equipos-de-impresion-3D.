import wifimgr
import publishThingspeak
import time

while(True):

	wlan = wifimgr.get_connection()

	if wlan is None:

		print("Could not initialize the network connection.")
		break

	try:

		publishThingspeak.calculateAndPublish()
		time.sleep(60)

	finally:

		print("exception")

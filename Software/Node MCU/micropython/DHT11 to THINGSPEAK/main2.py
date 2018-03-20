import wifimgr
import publishThingspeak

while(true):

	wlan = wifimgr.get_connection()

	if wlan is None:

		print("Could not initialize the network connection.")

	try:

		publishThingspeak.calculateAndPublish()
		time.sleep(12)

	finally:

		print("exception")


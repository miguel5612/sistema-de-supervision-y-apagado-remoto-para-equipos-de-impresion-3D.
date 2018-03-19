import network
import machine
wlan_sta = network.WLAN(network.STA_IF)

def confirmandreset():
	if not wlan_sta.isconnected():
		print("Excepcion, no tengo wifi.")
		machine.reset()
	print("La board tiene WiFi :), no me reiniciare")

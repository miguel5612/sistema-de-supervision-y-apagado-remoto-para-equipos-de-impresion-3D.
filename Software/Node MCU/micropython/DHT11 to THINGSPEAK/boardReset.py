import network
import machine
wlan_sta = network.WLAN(network.STA_IF)
global numError
def confirmandreset():
	if not wlan_sta.isconnected():
		resetBoardNow()
	elif confirmError():
		resetBoardNow()
	print("La board tiene WiFi :), no me reiniciare")
def confirmError():
	try:
		numError += 1
	except:
		numError = 1
	return errorCount(numError)
def errorCount(num):
	print("El numero de errores es: "+str(num))
	if(num>0):
		return True
	else:
		return False
def resetBoardNow():
	print("Excepcion, no tengo wifi.")
	machine.reset()

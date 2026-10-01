work= data.copy()

if work.get("lifetime",0) == 0 :
    work["lifetime"]=2

if work.get("repeat",0) == 0 :
   work["repeat"]=1

if work.get("name","") == "" :
   work["name"]="homeassistant"

hass.services.call('mqtt', 'publish', { "topic": "awtrix/customapp", "payload":str(work) }, False)
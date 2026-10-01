if work.get("repeat",0) == 0 :
   work["repeat"]=3

if work.get("name","") == "" :
   work["name"]="homeassistant"

if work.get("moveIcon","") == "" :
   work["moveIcon"]=True

work["repeatIcon"]=True
hass.services.call('mqtt', 'publish', { "topic": "awtrix/notify", "payload":str(work) }, False)
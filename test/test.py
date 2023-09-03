import server
import threading
import requests
import time
import json
import sys

botUrl = "http://localhost:8081/notification"
port = 8080
sep = "-"*25


class Unit:
    def __init__(self, fileName: str, description: str) -> None:
        self.fileName = fileName
        self.description = description


serverThread = threading.Thread(target=server.run, args=[port], daemon=True)
serverThread.start()

def executeUnit(fileName: str):
    with open(fileName) as f: 
        content = f.read()
        print("File:", fileName)
        print("Content:", content)
        response = requests.post(url=botUrl, json=json.loads(content))
        print(response.status_code)
        time.sleep(0.25)
        print(sep)

if len(sys.argv) <= 1:
    print("Wronk usage")

elif sys.argv[1] == "registration/trace1":
    units = [
        Unit("jsons/registration/trace1/1.json", ""),
        Unit("jsons/registration/trace1/2.json", ""),
        Unit("jsons/registration/trace1/3.json", ""),
        Unit("jsons/registration/trace1/4.json", ""),
        Unit("jsons/registration/trace1/5.json", ""),
        Unit("jsons/registration/trace1/6.json", ""),
        Unit("jsons/registration/trace1/7.json", ""),
        Unit("jsons/registration/trace1/8.json", ""),
        Unit("jsons/registration/trace1/9.json", ""),
        Unit("jsons/registration/trace1/10.json", ""),
        Unit("jsons/registration/trace1/11.json", ""),
        Unit("jsons/registration/trace1/12.json", ""),
        Unit("jsons/registration/trace1/13.json", ""),
        Unit("jsons/registration/trace1/14.json", ""),
        Unit("jsons/registration/trace1/15.json", ""),
        Unit("jsons/registration/trace1/16.json", ""),
        Unit("jsons/registration/trace1/17.json", ""),
        Unit("jsons/registration/trace1/18.1.json", ""),
        Unit("jsons/registration/trace1/18.2.json", ""),
        Unit("jsons/registration/trace1/23.json", ""),
        Unit("jsons/registration/trace1/24.json", ""),
        Unit("jsons/registration/trace1/25.json", ""),
        Unit("jsons/registration/trace1/26.1.json", ""),
        Unit("jsons/registration/trace1/26.2.json", ""),
        Unit("jsons/registration/trace1/27.json", ""),
        Unit("jsons/registration/trace1/28.json", ""),
        Unit("jsons/registration/trace1/29.1.json", ""),
        Unit("jsons/registration/trace1/29.2.json", ""),
        Unit("jsons/registration/trace1/30.0.json", ""),
        Unit("jsons/registration/trace1/30.1.json", ""),
        Unit("jsons/registration/trace1/31.json", ""),
        Unit("jsons/registration/trace1/32.json", ""),
        Unit("jsons/registration/trace1/33.json", ""),
        Unit("jsons/registration/trace1/34.json", ""),
        Unit("jsons/registration/trace1/35.json", ""),
        Unit("jsons/registration/trace1/36.json", ""),
        Unit("jsons/registration/trace1/36.5.json", ""),
        Unit("jsons/registration/trace1/37.json", ""),
        Unit("jsons/registration/trace1/38.json", ""),
        Unit("jsons/registration/trace1/39.json", ""),
        Unit("jsons/registration/trace1/40.json", ""),
        Unit("jsons/registration/trace1/41.json", ""),
        Unit("jsons/registration/trace1/42.json", ""),
        Unit("jsons/registration/trace1/43.json", ""),
        Unit("jsons/registration/trace1/44.json", ""),
        Unit("jsons/registration/trace1/45.json", ""),
        Unit("jsons/registration/trace1/46.json", ""),
    ]
    for unit in units:
        executeUnit(unit.fileName)

else:
    executeUnit(sys.argv[1])

time.sleep(3)
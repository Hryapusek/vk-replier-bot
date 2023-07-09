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
        # time.sleep(0.4)
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
        Unit("jsons/registration/trace1/19.json", ""),
        Unit("jsons/registration/trace1/20.json", ""),
        Unit("jsons/registration/trace1/21.json", ""),
        Unit("jsons/registration/trace1/22.json", ""),
    ]
    for unit in units:
        executeUnit(unit.fileName)

else:
    executeUnit(sys.argv[1])

time.sleep(1000)
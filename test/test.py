import server
import threading
import requests
import time
import json
import sys

botUrl = "http://localhost:8081/notification"
port = 8080

class Unit:
    def __init__(self, fileName: str, description: str) -> None:
        self.fileName = fileName
        self.description = description

serverThread = threading.Thread(target=server.run, args=[port], daemon=True)
serverThread.start()

units = [
    Unit("jsons/message_new/good/1.json", "No command message"),
    Unit("jsons/message_new/good/2.json", "No command message"),
    Unit("jsons/message_new/good/3.json", "No command message"),
    Unit("jsons/message_new/good/4.json", "No command message"),
    Unit("jsons/message_new/good/5.json", "No command message"),
]

if len(sys.argv) == 1:
    for unit in units:
        f = open(unit.fileName)
        content = f.read()
        f.close()
        print("File:", unit.fileName)
        print("Description:", unit.description)
        print("Content:", content)
        response = requests.post(url=botUrl, json=json.loads(content))
        print(response.status_code)
        time.sleep(0.7)
else:
    f = open(sys.argv[1])
    content = f.read()
    f.close()
    print("File:", sys.argv[1])
    print("Content:", content)
    response = requests.post(url=botUrl, json=json.loads(content))
    print(response.status_code)
    time.sleep(0.7)
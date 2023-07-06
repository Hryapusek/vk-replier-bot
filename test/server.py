from http.server import HTTPServer, BaseHTTPRequestHandler
import json

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):

  def do_GET(self):
      print(self.requestline)
      self.send_response(200)
      self.end_headers()
  
  def do_POST(self):
        self.printBody()
        self.send_response(200)
        self.end_headers()
  
  def printBody(self):
      content_length = int(self.headers['Content-Length'])
      body = self.rfile.read(content_length)
      my_json = body.decode('utf8').replace("'", '"')
      data = json.loads(my_json)
      s = json.dumps(data, indent=4, sort_keys=True)
      print(s)

def run(port: int):
    httpd = HTTPServer(('localhost', port), SimpleHTTPRequestHandler)
    httpd.serve_forever()
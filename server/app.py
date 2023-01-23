import os
import argparse
import json
import requests

import flask

parser = argparse.ArgumentParser()
parser.add_argument('-s', '--ssl', action='store_true', default=False)
args = parser.parse_args()


app = flask.Flask(__name__, instance_relative_config=True)
app.secret_key = os.environ.get("SECRET_KEY") or os.urandom(24)


# a simple page that says hello
@app.route('/power', methods=['POST', 'GET'])
def power():
  requests.get("http://192.168.1.184/motor_api?name=MCPWM0A&action=press")
  return '/power'

@app.route('/hue', methods=['POST', 'GET'])
def hue():
  requests.get("http://192.168.1.184/motor_api?name=MCPWM0B&action=press")
  return '/hue'

@app.route('/brighter', methods=['POST', 'GET'])
def brighter():
  requests.get("http://192.168.1.184/motor_api?name=MCPWM1A&action=press")
  return '/brighter'

@app.route('/dimmer', methods=['POST', 'GET'])
def dimmer():
  requests.get("http://192.168.1.184/motor_api?name=MCPWM1B&action=press")
  return '/dimmer'

if __name__ == '__main__':
  if args.ssl: 
    app.run('0.0.0.0', 5443, debug=True, ssl_context=("C:\certs\certificate.pem", "C:\certs\key.pem"))
  else:
    app.run('0.0.0.0', 5000, debug=True)
  
from flask import Flask
import requests
import json

app = Flask(__name__)


base_url = "http://onem2m.iiit.ac.in:443/~/in-cse/in-name/"

AQ = "AE-AQ/AQ-KN00-00"
SL = "AE-SL/SL-VN03-00"
srAC = "AE-SR/SR-AC/SR-AC-KH00-00"
srAQ = "AE-SR/SR-AQ/SR-AQ-KH00-01"
srEM = "AE-SR/SR-EM/SR-EM-KH04-00"
WE = "AE-WE/WE-VN04-00"
WD = "AE-WM/WM-WD/WM-WD-VN01-01"
WF = "AE-WM/WM-WF/WM-WF-VN04-70"
WN = "AE-WN/WN-L001-03"



headers = {
  "X-M2M-Origin": "iiith_guest:iiith_guest",
  "Content-Type": "application/json"
  }

# @app.route("/")
# def get_aq_data():
#     url = base_url + AQ + "/Data/la"
#     payload = ""
#     response = requests.request("GET", url, headers=headers, data=payload)
#     rec = json.loads(response.text)
#     # print(rec)
#     data_list = eval(rec["m2m:cin"]["con"])
#     # print(data_list[0:])
#     eas = {"PM2.5": data_list[1], "Calibrated PM2.5": data_list[2], "PM10": data_list[3], "Calibrated PM10": data_list[4], "Temperature": data_list[5], "Calibrated Temperature": data_list[6], "Relative Humidity": data_list[7], "Calibrated Relative Humidity": data_list[8], "AQI": data_list[9], "AQL": data_list[10], "AQI-MP": data_list[11], "Data Interval": data_list[12]}
#     # print(eas)
#     ret_val = " PM2.5 : " + str(data_list[2]) + " PM10 : " + str(data_list[4]) +  " Temperature : " + str(data_list[6]) + " Relative Humidity : " + str(data_list[8]) +  " AQI : " + str(data_list[9])
#     # print(ret_val)
#     final_ret = {
#        "list": eas,
#        "string": ret_val
#     }
#     ret = json.dumps(final_ret)
#     print(ret)
#     return ret
    
@app.route("/aq")
def get_aq_data():
    url = base_url + AQ + "/Data/la"
    payload = ""
    response = requests.request("GET", url, headers=headers, data=payload)
    rec = json.loads(response.text)
    # print(rec)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    # ['Timestamp', 'PM2.5', 'Calibrated PM2.5', 'PM10', 'Calibrated PM10', 'Temperature', 'Calibrated Temperature', 'Relative Humidity', 'Calibrated Relative Humidity', 'AQI', 'AQL', 'AQI-MP', 'Data Interval']
    eas = {"PM2.5": data_list[2], "PM10": data_list[4], "Temperature": data_list[5], "Relative Humidity": data_list[8], "AQI": data_list[9]}
    # print(eas)
    params = ["PM2.5","PM10","Temp"," rH","AQI"]
    values = [data_list[2],data_list[4],data_list[5],data_list[8],data_list[9]]
    ret_val = "PM2.5 : " + str(data_list[2]) + ", PM10 : " + str(data_list[4]) +  ", Temperature : " + str(data_list[5]) + ", Relative Humidity : " + str(data_list[8]) +  ", AQI : " + str(data_list[9])
    # print(ret_val)
    units = ["ug/m3","ug/m3","C","%Rh",""]
    final_ret = {
      "params" : params,
       "values": values,
       "units": units,
       "string": ret_val
    }
    ret = json.dumps(final_ret)
    print(ret)
    return ret






@app.route("/wd")
def get_WD_data():
  url = base_url + WD + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  # print(data_list[1:])
  eas = { "Temperature":data_list[1], "TDS Voltage":data_list[2], "Uncompensated TDS value":data_list[3], "Compensated TDS value":data_list[4]}
  # print(eas)
  string_data = "Temperature : " + str(data_list[1]) + ", TDS Voltage : " + str(data_list[2]) + ", Uncompensated TDS value : " + str(data_list[3]) +  ", Compensated TDS value : " + str(data_list[4])
  units = [""]
  # print(string_data)
  final_ret = {
       "list": eas,
       "string": string_data
  }
  ret = json.dumps(final_ret)
  print(ret)
  return ret
  
# get_WD_data()

# @app.route("/sl")
# def get_SL_data():
#   url = base_url + SL + "/Data/la"
#   payload = ""
#   response = requests.request("GET",url,headers=headers,data=payload)
#   rec = json.loads(response.text)
#   data_list = eval(rec["m2m:cin"]["con"])
#   print(data_list[1:])
#   return data_list
# "Timestamp" : data_list[0], "Eac_today": data_list[1], "Eac_total": data_list[2], "ActivePower": data_list[3], "Voltage_RS": data_list[], "Voltage_ST": data_list[], "Voltage_TR": data_list[], "Frequency": data_list[], "Power : data_list[]Factor", "Voltage1": data_list[], "Current1": data_list[], "Power1": data_list[], "Voltage2": data_list[], "Current2": data_list[], "Power2": data_list[], "Voltage3": data_list[], "Current3": data_list[], "Power3": data_list[], "PV1Voltage": data_list[], "PV1Current": data_list[], "PV1Power": data_list[], "PV2Voltage": data_list[], "PV2Current": data_list[], "PV2Power": data_list[], "PV3Voltage": data_list[], "PV3Current": data_list[], "PV3Power": data_list[], "PV4Voltage": data_list[], "PV4Current": data_list[], "PV4Power": data_list[], "PV5Voltage": data_list[], "PV5Current": data_list[], "PV5Power": data_list[], "PV6Voltage": data_list[], "PV6Current": data_list[], "PV6Power": data_list[]

if __name__ == "__main__":
  app.run("0.0.0.0",debug=True,port=80)
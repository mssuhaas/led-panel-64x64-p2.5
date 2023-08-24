# from fastapi import FastAPI
import requests
import json

base_url = "http://onem2m.iiit.ac.in:443/~/in-cse/in-name/"

AQ = "AE-AQ/AQ-MG00-00"
SL = "AE-SL/SL-VN03-00"
srAC = "AE-SR/SR-AC/SR-AC-KH00-00"
srAQ = "AE-SR/SR-AQ/SR-AQ-KH00-01"
srEM = "AE-SR/SR-EM/SR-EM-KH04-00"
WE = "AE-WE/WE-VN04-00"
WD = "AE-WM/WM-WD/WM-WD-VN01-01"
WF = "AE-WM/WM-WF/WM-WF-VN04-70"
WN = "AE-WN/WN-L001-03"

# app = FastAPI()

headers = {
  "X-M2M-Origin": "iiith_guest:iiith_guest",
  "Content-Type": "application/json"
  }

def get_energy_data():
  url = base_url + srEM + "/Data/la"
  payload = ""
  response = requests.request("GET", url, headers=headers, data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[0:])
  eas = {
    "Energy" : data_list[1],
    "Power" : data_list[2],
    "Current" : data_list[3]
  }
  print(eas)

# get_energy_data()

def get_aq_data():
  url = base_url + AQ + "/Data/la"
  payload = ""
  try:
    response = requests.request("GET", url, headers=headers, data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    eas = {"PM2.5": data_list[1], "Calibrated PM2.5": data_list[2], "PM10": data_list[3], "Calibrated PM10": data_list[4], "Temperature": data_list[5], "Calibrated Temperature": data_list[6], "Relative Humidity": data_list[7], "Calibrated Relative Humidity": data_list[8], "AQI": data_list[9], "AQL": data_list[10], "AQI-MP": data_list[11], "Data Interval": data_list[12]}
    print(eas)
  except Exception:
    print("ERROR!")

# get_aq_data()

def get_srAC_data():
  url = base_url + srAC + "/Data/la"
  payload = ""
  response = requests.request("GET", url, headers=headers, data=payload)
  rec = json.loads(response.text)
  print(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = {"Room Temp": data_list[1],"Temp Adjust": data_list[2],"Start Stop Status": data_list[3],"Alarm": data_list[4],"Malfunction Code": data_list[5],"Air Con Mode Status": data_list[6],"Air Flow Rate Status": data_list[7],"Filter Sign": data_list[8],"Gas Total Power": data_list[9],"Elec Total Power": data_list[10],"Air Direction Status": data_list[11],"Forced Thermo OFF Status": data_list[12],"Energy Efficiency Status": data_list[13],"Thermo Status": data_list[14],"Compressor Status": data_list[15],"Indoor Fan Status": data_list[16],"Heater Status": data_list[17]}
  print(eas)

# get_srAC_data()

def get_srAQ_data():
  url = base_url + srAQ + "/Data/la"
  payload = ""
  response = requests.request("GET", url, headers=headers, data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = {"CO2": data_list[1],"Temperature": data_list[2],"Relative Humidity": data_list[3]}
  print(eas)

# get_srAQ_data()

def get_srEM_data():
  url = base_url + srEM + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = { "Energy":data_list[1], "Power":data_list[2], "Current":data_list[3]}
  print(eas)

# get_srEM_data()

def get_WE_data():
  url = base_url + WE + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = { "Solar Radiation":data_list[1], "Temperature":data_list[2], "Relative Humidity":data_list[3], "Wind Direction":data_list[4], "Wind Speed":data_list[5], "Gust Speed":data_list[6], "Dew Point":data_list[7], "Battery DC Voltage":data_list[8], "Rain":data_list[9], "Pressure":data_list[10]}
  string_data = "Solar Radiation " + data_list[1] +  " Temperature " + data_list[2] +  " Relative Humidity " + data_list[3] +  " Wind Direction " + data_list[4] +  " Wind Speed " + data_list[5] +  " Gust Speed " + data_list[6] +  " Dew Point " + data_list[7] +  " Battery DC Voltage " + data_list[8] +  " Rain " + data_list[9] +  " Pressure":data_list[10]
  print(string_data)

get_WE_data()

def get_WD_data():
  url = base_url + WD + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = { "Temperature":data_list[1], "TDS Voltage":data_list[2], "Uncompensated TDS value":data_list[3], "Compensated TDS value":data_list[4]}
  print(eas)
  # string_data = "Temperature " + data_list[1] + " TDS Voltage " + data_list[2] + " Uncompensated TDS value " + data_list[3] +  " Compensated TDS value " + data_list[4]
  # print(string_data)

get_WD_data()

def get_WF_data():
  url = base_url + WF + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])
  eas = { "Flowrate":data_list[1],"Total Flow":data_list[2]}
  print(eas)

# get_WF_data()


# # GETTING AN ERROR!!! 
# def get_WN_data():
#   url = base_url + WN + "/Data/la"
#   payload = ""
#   response = requests.request("GET",url,headers=headers,data=payload)
#   rec = json.loads(response.text)
#   data_list = eval(rec["m2m:cin"]["con"])
#   print(data_list[1:])
#   eas = { "Node_Status":data_list[1], "Light_Status":data_list[2], "Rsl_Out":data_list[3], "Latency":data_list[4], "Data_Rate":data_list[5], "Mac_Address":data_list[6], "Packet_Size":data_list[7], "Rsl_In":data_list[8], "Etx":data_list[9], "Rpl_Rank":data_list[10], "Mac_tx_failed_count":data_list[11], "Mac_tx_count":data_list[12], "Neighbour":data_list[13]}
#   print(eas)

# # get_WN_data()


















def get_SL_data():
  url = base_url + SL + "/Data/la"
  payload = ""
  response = requests.request("GET",url,headers=headers,data=payload)
  rec = json.loads(response.text)
  data_list = eval(rec["m2m:cin"]["con"])
  print(data_list[1:])

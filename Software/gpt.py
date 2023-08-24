from fastapi import FastAPI
import requests
import time
from threading import Thread
import json

app = FastAPI()

data = {"AQ":None,"SL":None,"srAC": None,"srAQ": None,"srEM": None,"WE": None,"WD": None,"WF": None,"WN": None}

# _URL = "https://jsonplaceholder.typicode.com/todos/1"

base_url = "http://onem2m.iiit.ac.in:443/~/in-cse/in-name/"

AQ = "AE-AQ/AQ-KN00-00"
SL = "AE-SL/SL-VN03-00"
srAC = "AE-SR/SR-AC/SR-AC-KH00-00"
srAQ = "AE-SR/SR-AQ/SR-AQ-KH00-01"
srEM = "AE-SR/SR-EM/SR-EM-KH04-01"
WE = "AE-WE/WE-VN04-00"
WD = "AE-WM/WM-WD/WM-WD-VN01-01"
WF = "AE-WM/WM-WF/WM-WF-VN04-70"
WN = "AE-WN/WN-L001-03"

headers = {
  "X-M2M-Origin": "iiith_guest:iiith_guest",
  "Content-Type": "application/json"
  }

payload = ""


def get_aq_data():
    url = base_url + AQ + "/Data/la"
    payload = ""
    response = requests.request("GET", url, headers=headers, data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    # eas = {"PM2.5": data_list[2], "PM10": data_list[4], "Temperature": data_list[5], "Relative Humidity": data_list[8], "AQI": data_list[9]}
    params = ["PM2.5","PM10","Temp"," rH","AQI"]
    values = [data_list[2],data_list[4],data_list[5],data_list[8],data_list[9]]
    ret_val = "PM2.5 : " + str(data_list[2]) + ", PM10 : " + str(data_list[4]) +  ", Temperature : " + str(data_list[5]) + ", Relative Humidity : " + str(data_list[8]) +  ", AQI : " + str(data_list[9])
    units = ["ug/m3","ug/m3","C","%Rh"," "]
    final_ret = {
       "count" : len(params),
      "params" : params,
       "values": values,
       "units": units,
       "string": ret_val
    }
    return final_ret

def get_wd_data():
    url = base_url + WD + "/Data/la"
    payload = ""
    response = requests.request("GET", url, headers=headers, data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    # eas = { "Temp":data_list[1], "TDS Voltage":data_list[2], "Uncompensated TDS value":data_list[3], "Compensated TDS value":data_list[4]}
    params = ["Temp","TDSV","uTDS","cTDS"]
    values = [data_list[1],data_list[2],data_list[3],data_list[4]]
    ret_val = "Temperature : " + str(data_list[1]) + ", TDS Voltage : " + str(data_list[2]) + ", Uncompensated TDS : " + str(data_list[3]) + ", Compensated TDS : " + str(data_list[4])
    units = ["C"," V","ppm","ppm"]
    final_ret = {
       "count" : len(params),
      "params" : params,
       "values": values,
       "units": units,
       "string": ret_val
    }
    return final_ret

def get_srEM_data():
    url = base_url + srEM + "/Data/la"
    payload = ""
    response = requests.request("GET",url,headers=headers,data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    data_list[1] = int(data_list[1]/1000)
    # eas = { "Energy":data_list[1], "Power":data_list[2], "Current":data_list[3]}
    params = ["Energy","Power","Current"]
    values = [data_list[1],data_list[2],data_list[3]]
    units = ["kWh","kW","A"]
    ret_val = "Energy : " + str(data_list[1])+", Power : " + str(data_list[2])+", Current : " + str(data_list[3])
    final_ret = {
    "count" : len(params),
    "params" : params,
    "values": values,
    "units": units,
    "string": ret_val
    }
    return final_ret


def get_WN_data():
    url = base_url + + "Data/la"
    payload = ""
    response = requests.get("GET",url=url,headers=headers,data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    params = 
    values = 
    units = 
    ret_val = 
    final_ret = {
        "count" : len(params),
        "params": params,
        "values": values,
        "units": units,
        "string": ret_val
    }
    return final_ret


def get_SL_data():
    url = base_url + + "Data/la"
    payload = ""
    response = requests.get("GET",url=url,headers=headers,data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    params = 
    values = 
    units = 
    ret_val = 
    final_ret = {
        "count" : len(params),
        "params": params,
        "values": values,
        "units": units,
        "string": ret_val
    }
    return final_ret


def get_WF_data():
    url = base_url + + "Data/la"
    payload = ""
    response = requests.get("GET",url=url,headers=headers,data=payload)
    rec = json.loads(response.text)
    data_list = eval(rec["m2m:cin"]["con"])
    print(data_list[0:])
    params = 
    values = 
    units = 
    ret_val = 
    final_ret = {
        "count" : len(params),
        "params": params,
        "values": values,
        "units": units,
        "string": ret_val
    }
    return final_ret


def fetch_data():
    global data
    while True:
        try:
            data["AQ"] = get_aq_data()
            data["WD"] = get_wd_data()
            data["srEM"] = get_srEM_data()
            # data["WN"] = get_WN_data()
            # data["SL"] = get_SL_data()
            # data["WF"] = get_WF_data()
            
            print("Data fetched successfully!")
        except Exception as e:
            print(f"An error occurred: {e}")
        time.sleep(60)

data_fetch_thread = Thread(target=fetch_data)
data_fetch_thread.daemon = True
data_fetch_thread.start()

# @app.get("/")



@app.get("/aq")
def r_aq():
    return data["AQ"]

@app.get("/wd")
def r_wd():
    return data["WD"]

@app.get("/srEM")
def r_srEM():
    return data["srEM"]


if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="0.0.0.0", port=8000)

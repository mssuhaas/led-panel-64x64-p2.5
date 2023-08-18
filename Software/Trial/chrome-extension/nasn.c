#include <stdio.h>
#include <string.h>
#include <stdlib.h> // Include the standard library for memory allocation

#define AirQuality 0
#define WaterQuality 1
#define Weather 2
#define Solar 3 
#define Energy 4 
#define SmartRoom 5 
#define WiSUN 6 
#define Crowd 7

char* extract_data_string_parameters(const char *xmlString) {
    const char *startTag = "<str name=\"Data String Parameters\" val=\"";
    const char *endTag = "\"/>";
    const char *nameStart = strstr(xmlString, startTag);

    if (nameStart != NULL) {
        nameStart += strlen(startTag);
        const char *valueStart = nameStart;
        const char *valueEnd = strstr(valueStart, endTag);

        if (valueEnd != NULL) {
            int length = valueEnd - valueStart;
            char *result = (char *)malloc(length + 1); // +1 for null-terminator

            if (result != NULL) {
                strncpy(result, valueStart, length);
                result[length] = '\0'; // Null-terminate the string
                return result;
            }
        }
    }

    return NULL; // Return NULL if extraction fails
}

void extractColumnNames(const char *input_string, char column_names_array[][50], int max_columns) {
    char *start = strchr(input_string, '\'');
    int index = 0;

    while (start != NULL && index < max_columns) {
        char *end = strchr(start + 1, '\'');
        if (end != NULL) {
            strncpy(column_names_array[index], start + 1, end - start - 1);
            column_names_array[index][end - start - 1] = '\0';
            index++;
            start = strchr(end + 1, '\'');
        } else {
            break;
        }
    }
}

void printColumnNames(const char column_names_array[][50], int num_columns) {
    for (int i = 0; i < num_columns; i++) {
        printf("%s\n", column_names_array[i]);
    }
}


int main() {
    char column_names_array[20][50];
    const char xmlString[] ="<obj><str name=\"Node ID\" val=\"WN-L001-03\"/>\n<str name=\"Node Location\" val=\"{'Latitude': 17.445050827321868, 'Longitude': 78.34991949925372}\"/>\n<str name=\"Device Model\" val=\"{'Controller': 'Arduino_nano_V3 interfaced with Radio Board:EFR32MG12', 'Device': 'Wi-SUN Smart pole with Node status, Light status, Rsl_out, Latency, Data rate, Mac address ,Packet size, Rsl_in, etx, Rpl_rank, Mac_tx_failed_count, Mac_tx_count, Neighbour', 'Sensors': ['(Timestamp)', '(Node_Status = EFR32MG12, id=1.0)', '(Light_Status = EFR32MG12, id=1.0)', '(Rsl_Out = EFR32MG12, id=1.0)', '(Latency = EFR32MG12, id=1.0)', '(Data_Rate = EFR32MG12, id=1.0)', '(Mac_Address = EFR32MG12, id=1.0)', '(Packet_Size = EFR32MG12, id=1.0)', '(Rsl_In = EFR32MG12, id=1.0)', '(Etx = EFR32MG12, id=1.0)', '(Rpl_Rank = EFR32MG12, id=1.0)', '(Mac_tx_failed_count = EFR32MG12, id=1.0)', '(Mac_tx_count = EFR32MG12, id=1.0)', '(Neighbour = EFR32MG12, id=1.0)']}\"/>\n<str name=\"Version History\" val=\"[{'ver': 'V1.0.0', 'dt_start': '30-08-2022 00-00-00', 'dt_end': '25-04-2023 00-00-00'}, {'ver': 'V2.0.0', 'dt_start': '25-04-2023 00-00-00', 'dt_end': '30-12-9999 00-00-00'}]\"/>\n<str name=\"Data String Parameters\" val=\"['Timestamp', 'Node_Status', 'Light_Status', 'Rsl_Out', 'Latency', 'Data_Rate', 'Mac_Address', 'Packet_Size', 'Rsl_In', 'Etx', 'Rpl_Rank', 'Mac_tx_failed_count', 'Mac_tx_count', 'Neighbour']\"/>\n<str name=\"Parameters Description\" val=\"Data Description, [datatype], [Units], [Resolution], [Accuracy]\"/>\n<str name=\"Timestamp\" val=\"The number of seconds that have elapsed since Thursday, 1970 Jan 1 00:00:00 UTC, [int], [s], [60 s], [n/a]\"/>\n<str name=\"Node_Status\" val=\"The instantaneous value of node [L001on = on] or [L001off = off], [String], [n/a], [n/a], [n/a]\"/>\n<str name=\"Light_Status\" val=\"The instantaneous value of Light [L001on = on] or [L001off = off], [String], [n/a], [n/a], [n/a]\"/>\n<str name=\"Rsl_out\" val=\"The Received Signal Level out is an EWMA of the received signal level for the node-to-neighbor direction. provides a range of -174 (0)to +80 (254) dBm, [float], [dbm], [0.1 dbm], [n/a]\"/>\n<str name=\"Latency\" val=\"The instantaneous value of latency having minimum [0.2] sec to maximum[5] sec, [float], [sec], [0.1 sec], [n/a]\"/>\n<str name=\"Data_Rate\" val=\"The instantaneous rate of data is 50kbps, [int], [kbps], [1 kbps], [n/a]\"/>\n<str name=\"Mac_Address\" val=\"fd12:3456::5232:5fff:fe42:61c6, [n/a], [n/a], [n/a], [n/a]\"/>\n<str name=\"Packet_Size\" val=\"The size of a packet is 50 bytes, [int], [bytes], [n/a], [n/a]\"/>\n<str name=\"Rsl_in\" val=\"The Received Signal Level in is an EWMA of the received signal level for the neighbor-to-node direction. This provides a range of -174 (0) to +80 (254) dBm in 1 dBm steps, [float], [dbm], [0.1 dbm], [n/a]\"/>\n<str name=\"etx\" val=\"Expected Transmission Count is an Exponentially Weighted Moving Average of the number of expected packet transmissions required for error-free reception at destination,[float], [n/a], [n/a], [n/a]\"/>\n<str name=\"Rpl_rank\" val=\"RPL rank represents the rank or hop distance from the tree root , [float], [n/a], [n/a], [n/a]\"/>\n<str name=\"Mac_tx_failed_count\" val=\"MAC transmit failed represents the number of unsuccessful transmission attempts, [float], [n/a], [n/a], [n/a]\"/>\n<str name=\"Mac_tx_count\" val=\"MAC transmission count represents the number of packets transmitted over the air, [float], [n/a], [n/a], [n/a]\"/>\n<str name=\"Neighbour\" val=\"Lifetime is a counter maintained with each neighbor to evaluate the state of the connection with this device lifetime, [String], [n/a], [n/a], [n/a]\"/>\n</obj>"; // Your XML-like string here // Your XML-like string here
    char *dataString = extract_data_string_parameters(xmlString);
    printf("%s",dataString);
    extractColumnNames(dataString,column_names_array,9);
    printColumnNames(column_names_array,sizeof(column_names_array)/sizeof(column_names_array[0]));
    free(dataString);
    return 0;
}



# OBD-LOGGER
By Uttam Deshpande et. al.,

OBD Logger is a data logger which provides access to data from the engine control unit (ECU) and offers a valuable source of information when troubleshooting problems inside a vehicle. The SAE J1979 standard defines a method for requesting various diagnostic data and a list ofstandard parameters that might be available from the ECU. The various parameters that are available are addressed by "parameter identification numbers" or PIDs which are defined in J1979. Manufacturers are not required to implement all PIDs listed in J1979 and they are allowed to include proprietary PIDs that are not listed. The PID request and data retrieval system gives access to real time performance data as well as flagged DTCs.
* Visit the following link for more details [Med Logger - Remote Patient Monitoring Using CAN Protocol](http://www.ijaresm.com/med-logger-remote-patient-monitoring-using-can-protocol)

## Introduction
OBD Logger is a data logger which is an electronic device which stores data over time. It helps in determining the status of vehicle’s various sub-systems. Introduction to OBD On-board diagnostics (OBD) is an automotive term referring to a vehicle's self-diagnostic and reporting capability. OBD systems give the vehicle owner or repair technician access to the status of the various vehicle sub-systems.
![image](https://user-images.githubusercontent.com/107185323/198992229-274e376a-5fcd-4830-ad38-47c85ec6c960.png)

The repository includes:
* Microvsion Keil Source code for OBD Data Logger.
* Visual Studio, C code.

## Citing
@ARTICLE{ AUTHOR={Deshpande, Uttam U. et al.,}, TITLE={Self-health Monitoring and Reporting System for COVID-19 Patients Using CAN Data Logger}, JOURNAL={Journal of Computer Science Research}, VOLUME={3}, YEAR={2021}, URL={ http://www.ijaresm.com/med-logger-remote-patient-monitoring-using-can-protocol } }

Requirements: Software
* Microvsion Keil
* Visual Studio

Requirements: Hardware
* STM32F407VGTx Microcontroller
* MCP2551 CAN transceiver
* MCP2515 CAN shield


## Results and discussion
The OBD Logger GUI gives user a convenient way to access the various PIDs of mode 1 diagnostic service. When the user clicks on a particular service, say for example Vehicle Speed, the PID associated with it is transmitted. The ECU responds with a response message which gets stored in the receive register. This is further sent to the GUI where the user can visualize the result.

![image](https://user-images.githubusercontent.com/107185323/198992412-d68320ff-e6fb-4520-89c5-f3168ba6ea47.png)

![image](https://user-images.githubusercontent.com/107185323/198992487-86a259ff-7ec7-43db-85a6-c75ba5f0ebeb.png)

![image](https://user-images.githubusercontent.com/107185323/198992539-4b1acb7a-883c-4ff0-a8ff-2217dd80cf6c.png)


## Usage
The vehicle diagnostics is achieved by requesting the current vehicle data or parameter ID’S in mode 1 diagnostic service of OBD-II standard. To read the ECU values from the vehicle CAN communication is used by writing driver API’s. The CAN controller model has been programmed for some of the specifications given in the datasheet. A graphic user interface (GUI) is developed for better visualization and making the software development work easy. This model emulates the behavior of vehicle diagnostics by being able to log the vehicle data in the GUI using CAN communication protocol.



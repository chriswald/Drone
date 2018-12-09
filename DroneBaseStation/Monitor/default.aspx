<%@ Page Language="C#" AutoEventWireup="true" CodeBehind="default.aspx.cs" Inherits="Monitor.Dashboard" %>

<!DOCTYPE html>

<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
    <title>Dashboard</title>

    <link rel="stylesheet" href="Content/Styles/style.css" />
    <script src="Scripts/ReplaceSvg.js"></script>
    <script src="Scripts/AppInsights.js"></script>
    <script src="Scripts/Controller.js"></script>
    <script src="Scripts/ControllerStreamer.js"></script>
    <script src="Scripts/ControllerVisualization.js"></script>
    <script src="Scripts/Battery.js"></script>
    <script src="Scripts/ConnectedDevice.js"></script>
    <script src="Scripts/Service.js"></script>
    <script src="Scripts/ComPorts.js"></script>
    <script src="Scripts/Main.js"></script>
</head>
<body>
    <form id="Dashboard" runat="server">
        <div>
            <div id="imagelist">
                <img src="Content/Images/batteryfull.svg" id="batteryfull" class="svg" />
                <img src="Content/Images/batterymedium.svg" id="batterymedium" class="svg" />
                <img src="Content/Images/batterylow.svg" id="batterylow" class="svg" />
                <img src="Content/Images/batteryempty.svg" id="batteryempty" class="svg" />
                <img src="Content/Images/controller.svg" id="controller" class="svg" />
                <img src="Content/Images/laptop.svg" id="laptop" class="svg" />
                <img src="Content/Images/plane.svg" id="plane" class="svg" />
                <img src="Content/Images/connected.svg" id="connected" class="svg" />
                <img src="Content/Images/radio.svg" id="radio" class="svg" />
            </div>

            <div>
                <span>Controller Service:</span>
                <span id="serviceStatus"></span>
                <br />
                <select id="comList" name="comId">
                </select>
                <button id="refreshPorts" type="button">Refresh COM Ports</button>
                <button id="startService" type="button">Start Service</button>
                <button id="stopService" type="button">Stop Service</button>
            </div>

            <table>
                <tr>
                    <td><svg class="svg"><use href="#controller"></use></svg></td>
                    <td><span>Controller</span></td>
                    <td><div id="controllerStatus"></div></td>
                    <td><div id="controllerBattery"></div></td>
                </tr>

                <tr>
                    <td><svg class="svg"><use href="#laptop"></use></svg></td>
                    <td><span>Laptop</span></td>
                    <td><div id="laptopStatus"></div></td>
                    <td><div id="laptopBattery"></div></td>
                </tr>

                <tr>
                    <td><svg class="svg"><use href="#radio"></use></svg></td>
                    <td><span>Radio Relay</span></td>
                    <td><div id="radioStatus"></div></td>
                    <td><div id="radioBattery"></div></td>
                </tr>

                <tr>
                    <td><svg class="svg"><use href="#plane"></use></svg></td>
                    <td><span>Drone</span></td>
                    <td><div id="droneStatus"></div></td>
                    <td><div id="droneBattery"></div></td>
                </tr>
            </table>

            <div id="controllerVisualization" style="width:550px;height:200px;"></div>
        </div>
    </form>
</body>
</html>

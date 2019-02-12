<style>
<?php include 'css/view_devices.css'; ?>
</style>
<?php
function printResult($devices, $rooms, $plugins) {
    //Display how many results there were
    #echo "$res->num_rows entries<br>";

    //Display each row
    $devices->data_seek(0);
    echo "<table id=\"devices\" class=\"devices\">";

    echo "<tr class=\"headers\">";

    #echo "<th class=\"device_id\">";
    #echo "device_id";
    #echo "</th>";
    echo "<th class=\"mac_address\" onclick=\"sortTable(0)\">";
    echo "MAC Address";
    echo "</th>";
    echo "<th class=\"plugin_name\" onclick=\"sortTable(1)\">";
    echo "Plugin";
    echo "</th>";
    echo "<th class=\"room_name\" onclick=\"sortTable(2)\">";
    echo "Resource";
    echo "</th>";
    echo "<th class=\"voltage\" onclick=\"sortTable(3)\">";
    echo "Voltage";
    echo "</th>";
    echo "<th class=\"firmware_version\" onclick=\"sortTable(4)\">";
    echo "Firmware Version";
    echo "</th>";
    echo "<th class=\"last_checked_in\" onclick=\"sortTable(5)\">";
    echo "Last Checked In";
    echo "</th>";
    echo "<th class=\"batteries_replaced_date\" onclick=\"sortTable(6)\">";
    echo "Batteries Replaced Date";
    echo "</th>";
    if (file_exists("../log/errors.log")) {
        echo "<th class=\"recent_errors\" onclick=\"sortTable(7)\">";
        echo "# of Recent Errors";
        echo "</th>";
    }

    echo "</tr>";

    //open error logs
    if (file_exists("../log/errors.log")) {
        $errorLogFile = file("../log/errors.log"); 
        $errorLogs = array();
        foreach ($errorLogFile as $entry) {
            if ($entry !== "") {
                $macAddress = substr($entry,0,12);
                $numErrors = substr($entry,13);
                $errorLogs[$macAddress] = $numErrors;
            }
        }
    }

    while ($device = $devices->fetch_assoc()) {
        echo "<tr class=\"device\" onclick=\"document.location = 'edit_device.php?device_id=$device[device_id]'\">";

        #echo "<td class=\"device_id\">";
        #echo $row["device_id"];
        #echo "</td>";
        echo "<td class=\"mac_address";
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["mac_address"];
        echo "</td>";
        echo "<td class=\"plugin_name";
        echo "\">";
        if (isset($plugins[$device['plugin']])) {
            echo $plugins[$device['plugin']]->getName();
        } else {
            echo "Error: Plugin not active";
        }
        echo "</td>";
        echo "<td class=\"room_name";
        echo "\">";
        if (isset($plugins[$device['plugin']])) {
            echo $rooms[$device['plugin']][$device['resource_id']];
        } else {
            echo "Error: Plugin not active";
        }
        echo "</td>";
        echo "<td class=\"voltage";
        if ($device["voltage"] < 2.5) {
            echo " orange";
        }
        echo "\">";
        echo $device["voltage"];
        echo "</td>";
        echo "<td class=\"firmware_version";
        echo "\">";
        echo $device["firmware_version"];
        echo "</td>";
        echo "<td class=\"last_checked_in";
        if (substr($device["last_checked_in"], 0, 10) !== date('Y-m-d') && $device['is_production']) {
            echo " orange";
        }
        if (!$device['is_production']) {
            echo " notproduction";
        }
        echo "\">";
        echo $device["last_checked_in"];
        echo "</td>";
        echo "<td class=\"batteries_replaced_date";
        echo "\">";
        echo $device["batteries_replaced_date"];
        echo "</td>";
        if (file_exists("../log/errors.log")) {
            echo "<td class=\"recent_errors";
            $recentErrors = $errorLogs[$device['mac_address']];
            if ($recentErrors > 5 && $device['is_production']) {
                echo " orange";
            }
            if (!$device['is_production']) {
                echo " notproduction";
            }
            echo "\">";
            echo $recentErrors;
            echo "</td>";
        }

        echo "</tr>";
    }
    echo "<tr class=\"fake_device\" onclick=\"document.location = 'edit_device.php?device_id=new'\">";
    echo "<td>Add New Device</td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "</tr>";
    echo "<tr class=\"fake_device\" onclick=\"document.location = 'voltage_charts.php'\">";
    echo "<td>Battery History</td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "<td></td>";
    echo "</tr>";

    echo "</table>";
}

require_once("../config/dbconfig.php");
if ($config->debugModeActive == "true") {
    error_reporting(E_ALL);
    ini_set('display_errors', '1');
}
$mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
$devices = mysqli_query($mysqli, "SELECT * FROM devices");
$rooms = array();
foreach (glob("../plugins/*.php") as $filename) {
    require_once($filename);
}
foreach ($plugins as $plugin) {
    $rooms[ $plugin->getIndex() ] = $plugin->getResources($config);
}
echo "<script src='js/view_devices.js'></script>";
printResult($devices, $rooms, $plugins);
?>

<style>
<?php include 'css/edit_device.css'; ?>
</style>
<?php
    require('../config/dbconfig.php');
    foreach (glob("../plugins/*.php") as $filename) {
        require_once($filename);
    }
    #Debug stuff
    if ($config->debugModeActive == "true") {
        error_reporting(E_ALL);
        ini_set('display_errors', '1');
    }
    $device_id = $_GET["device_id"];
    if ($_GET["device_id"] == "new") {
        $device = array(
            "mac_address" => "0A1B2C3D4E5F",
            "resource_id" => "",
            "orientation" => 1,
            "device_type" => 7,
            "voltage" => 7,
            "width" => 800,
            "height" => 480,
            "plugin" => 0,
            "qr" => true,
            "refresh" => 30,
            "is_production" => true,
            "only_events" => false,
            "notes" => ""
        );
        $plugin = reset($plugins);
        $device["plugin"] = $plugin->getIndex();
        $resources = $plugin->getResources($config);
        $device["resource_id"] = key($resources);
    } else if (preg_match('/^[[:digit:]]+$/', $device_id) === 1) {
        $mysqli = mysqli_connect($config->deviceDatabaseServer, $config->deviceDatabaseUsername, $config->deviceDatabasePassword, $config->deviceDatabaseName);
        $result = mysqli_query($mysqli, "SELECT * FROM devices WHERE device_id = $device_id");
        $device = mysqli_fetch_assoc($result);
    }
    $roomsArray = array();
    foreach ($plugins as $plugin) {
        $roomsArray[$plugin->getName()] = $plugin->getResources($config);
    }

    echo "<script language='javascript'>";
    echo "defaults = []";
    echo "</script>";

    echo "<div>";
    echo "<form id=\"form\" action=\"handle_edit_device.php\" method=\"post\">";
        echo "<input type=\"hidden\" name=\"new_device_id\" value=\"$device_id\"/>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_mac_address\">Mac Address:</label>";
            echo "<input type=\"text\" id=\"mac_address\" name=\"new_mac_address\" value=\"$device[mac_address]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_width\">Width:</label>";
            echo "<input type=\"text\" id=\"width\" name=\"new_width\" value=\"$device[width]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_height\">Height:</label>";
            echo "<input type=\"text\" id=\"height\" name=\"new_height\" value=\"$device[height]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_refresh\">Refresh (minutes):</label>";
            echo "<input type=\"text\" id=\"refresh\" name=\"new_refresh\" value=\"$device[refresh]\">";
        echo "</div>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_plugin\">Plugin:</label>";
            echo "<select id=\"plugin\" name=\"new_plugin\">";
                foreach ($plugins as $plugin) {
                    echo "<option value=\"";
                    echo $plugin->getIndex();
                    echo "\"";
                    if ($device["plugin"] == $plugin->getIndex()) {
                        echo " selected";
                    }
                    echo ">";
                    echo $plugin->getName();
                    echo "</option>";
                }
            echo "</select>";
        echo "</div>";
        foreach ($plugins as $plugin) {
            echo "<div class=\"field resource";
            if ($device["plugin"] != $plugin->getIndex()) {
                echo " hidden";
            }
            echo "\" data-pluginid=\"";
            echo $plugin->getIndex();
            echo "\">";
            echo "<label for=\"new_resource_id\">";
            echo $plugin->getName();
            echo " resource:</label>";
                echo "<select class=\"new_resource_id\" name=\"new_resource_id\">";
                foreach ($roomsArray[$plugin->getName()] as $key=>&$room) {
                    echo "<option value=\"";
                    echo $key;
                    echo "\"";
                    if ($key == $device["resource_id"]) {
                        echo " selected";
                    }
                    echo ">";
                    echo $room;
                    echo "</option>";
                }
                echo "</select>";
            echo "</div>";
        }
        foreach ($plugins as $plugin) {
            echo $plugin->getDeviceType($device);
        }
        echo "<fieldset id=\"orientation\" class=\"field\">";
            echo "<legend>Display Orientation</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"right-side_up\">Right-Side Up</label>";
                    echo "<input type=\"radio\" id=\"orientation_0\" name=\"new_orientation\" value=\"0\"";
                    if ($device['orientation'] == 0) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"up-side_down\">Up-Side Down</label>";
                    echo "<input type=\"radio\" id=\"orientation_1\" name=\"new_orientation\" value=\"1\"";
                    if ($device['orientation'] == 1) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<fieldset id=\"qr\" class=\"field\">";
            echo "<legend>Show QR code:</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"true\">True</label>";
                    echo "<input type=\"radio\" id=\"qr\" name=\"new_qr\" value=\"true\"";
                    if ($device['qr'] == true) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"false\">False</label>";
                    echo "<input type=\"radio\" id=\"qr\" name=\"new_qr\" value=\"false\"";
                    if ($device['qr'] == false) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<fieldset id=\"events\" class=\"field\">";
            echo "<legend>Only Display Scheduled Events:</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"true\">True</label>";
                    echo "<input type=\"radio\" id=\"onlyevents\" name=\"new_onlyevents\" value=\"true\"";
                    if ($device['only_events'] == true) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"false\">False</label>";
                    echo "<input type=\"radio\" id=\"onlyevents\" name=\"new_onlyevents\" value=\"false\"";
                    if ($device['only_events'] == false) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<fieldset id=\"production\" class=\"field\">";
            echo "<legend>Device is in production:</legend>";
            echo "<ul>";
                echo "<li>";
                    echo "<label for=\"true\">True</label>";
                    echo "<input type=\"radio\" id=\"isproduction\" name=\"new_production\" value=\"true\"";
                    if ($device['is_production'] == true) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
                echo "<li>";
                    echo "<label for=\"false\">False</label>";
                    echo "<input type=\"radio\" id=\"isnotproduction\" name=\"new_production\" value=\"false\"";
                    if ($device['is_production'] == false) {
                        echo " checked";
                    }
                    echo ">";
                echo "</li>";
            echo "</ul>";
        echo "</fieldset>";
        echo "<div class=\"field\">";
            echo "<label for=\"new_notes\">Notes:</label>";
            echo "<textarea id=\"notes\" name=\"new_notes\">$device[notes]</textarea>";
        echo "</div>";
        echo "<div class=\"button\">";
            echo "<button type=\"button\" onclick=\"window.location.href='view_devices.php'\">Cancel</button>";
            echo "<button type=\"button\" class=\"middle\" onclick=\"deleteLists(); document.getElementById('form').submit()\">Submit</button>";
            if ($_GET["device_id"] != "new") {
                echo "<button type=\"button\" onclick=\"if (confirm('Are you sure you want to delete this device?') == true) window.location.href='handle_delete_device.php?device_id=$device[device_id]'\">Delete</button>";
            }
        echo "</div>";
    echo "</form>";

    echo "</div>";

    echo "<div class=\"images\">";
    $imgurl = str_replace(" ", "%20", "../get_png.php?mac_address=$device[mac_address]&layout=$device[device_type]&plugin=$device[plugin]&resource_id=$device[resource_id]&voltage=$device[voltage]&width=$device[width]&height=$device[height]");
    echo "<img id=\"preview\" src=$imgurl />"; 
    if ($_GET["device_id"] != "new") {
        $batteryWeekImage = "$_SERVER[DOCUMENT_ROOT]/voltage_monitor/data/week_$device[mac_address].png";
        $batteryMonthImage = "$_SERVER[DOCUMENT_ROOT]/voltage_monitor/data/month_$device[mac_address].png";
        $batteryYearImage = "$_SERVER[DOCUMENT_ROOT]/voltage_monitor/data/year_$device[mac_address].png";
        if (file_exists($batteryWeekImage)) {
            echo "<img id=\"preview\" src=../voltage_monitor/data/week_$device[mac_address].png>"; 
        }
        if (file_exists($batteryMonthImage)) {
            echo "<img id=\"preview\" src=../voltage_monitor/data/month_$device[mac_address].png>"; 
        }
        if (file_exists($batteryYearImage)) {
            echo "<img id=\"preview\" src=../voltage_monitor/data/year_$device[mac_address].png>"; 
        }
        echo "<a href=\"view_logs.php?mac_address=$device[mac_address]&device_id=$device[device_id]\">View Logs</a>";
    }
    echo "</div>";
    echo "<script src=\"js/edit_device.js\">";
    echo "</script>";
    echo "<script language=javascript>";
    echo 'document.getElementById("type_"+defaults[plugin.value]+"_"+plugin.value).checked = true;';
    echo "</script>";
?>

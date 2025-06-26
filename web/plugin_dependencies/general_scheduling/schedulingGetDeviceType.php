<?php
function schedulingGetDeviceType($device, $pluginIndex) {
    //if necessary, set default
    //$validDeviceTypes = array(1,2,3,4,6,7,9,10);
    $validDeviceTypes = array(0,2,3,7,10,11,12,13);
    if (!in_array($device["device_type"], $validDeviceTypes)) {
        $device["device_type"] = 7;
    }
    $getDeviceType = "";
    $getDeviceType .= "<script language='javascript'>";
    $getDeviceType .= "defaults[" . $pluginIndex . "]=" . $device["device_type"] . ";";
    $getDeviceType .= "</script>";

    $getDeviceType .= "<fieldset class=\"field getdevicetype";
    if ($device['plugin'] != $pluginIndex) {
        $getDeviceType .= " hidden";
    }
    $getDeviceType .= "\" data-pluginid=\"";
    $getDeviceType .= $pluginIndex;
    $getDeviceType .= "\">";
        $getDeviceType .= "<legend>Device Type</legend>";
        $getDeviceType .= "<ul>";
        if (in_array(1, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"1\">4\" Landscape 1, 30 minute refresh cycle</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_1_$pluginIndex\" name=\"new_device_type\" value=\"1\"";
                #if ($device['device_type'] == 1 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 1) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(4, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"4\">4\" Landscape 2, 30 minute refresh cycle</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_4_$pluginIndex\" name=\"new_device_type\" value=\"4\"";
                #if ($device['device_type'] == 4 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 4) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(6, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"6\">4\" Landscape 3 (with QR code), 30 minute refresh cycle</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_6_$pluginIndex\" name=\"new_device_type\" value=\"6\"";
                #if ($device['device_type'] == 6 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 6) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(9, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"9\">4\" Landscape 4 (with QR code), 30 minute refresh cycle</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_9_$pluginIndex\" name=\"new_device_type\" value=\"9\"";
                #if ($device['device_type'] == 9 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 9) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(0, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"0\">7\" Portrait, all events, 30 minute refresh</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_0_$pluginIndex\" name=\"new_device_type\" value=\"0\"";
                #if ($device['device_type'] == 0 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 0) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(2, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"2\">7\" Landscape 2 events, QR code, 15 minute refresh</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_2_$pluginIndex\" name=\"new_device_type\" value=\"2\"";
                #if ($device['device_type'] == 2 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 2) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(3, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"3\">7\" Landscape 2+ events, QR code, 15 minute refresh</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_3_$pluginIndex\" name=\"new_device_type\" value=\"3\"";
                #if ($device['device_type'] == 3 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 3) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(7, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"7\">7\" Landscape 2 events, QR code, 30 minute refresh</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_7_$pluginIndex\" name=\"new_device_type\" value=\"7\"";
                #if ($device['device_type'] == 7 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 7) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(10, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"10\">7\" Landscape 2 events, QR code, 30 minute refresh cycle, better battery</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_10_$pluginIndex\" name=\"new_device_type\" value=\"10\"";
                #if ($device['device_type'] == 10 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 10) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(11, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"11\">7\" Landscape for event spaces, 30 minute refresh cycle</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_11_$pluginIndex\" name=\"new_device_type\" value=\"11\"";
                #if ($device['device_type'] == 11 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 11) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(12, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"12\">7\" Landscape 2 events, QR code, 15 minute refresh, no bottom bar</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_12_$pluginIndex\" name=\"new_device_type\" value=\"12\"";
                #if ($device['device_type'] == 12 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 12) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        if (in_array(13, $validDeviceTypes)) {
            $getDeviceType .= "<li>";
                $getDeviceType .= "<label for=\"13\">7\" Landscape 2+ events, QR code, 30 minute refresh</label>";
                $getDeviceType .= "<input type=\"radio\" id=\"type_13_$pluginIndex\" name=\"new_device_type\" value=\"13\"";
                #if ($device['device_type'] == 13 && $device['plugin'] == $pluginIndex) {
                if ($device['device_type'] == 3) {
                    $getDeviceType .= " checked";
                }
                $getDeviceType .= ">";
            $getDeviceType .= "</li>";
        }
        $getDeviceType .= "</ul>";
    $getDeviceType .= "</fieldset>";
    return $getDeviceType;
}
?>

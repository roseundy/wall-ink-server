<?php
require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/iPlugin.php");
require("$_SERVER[DOCUMENT_ROOT]/config/dbconfig.php");

class planningCenterPlugin implements iPlugin {
    private function trimResourceName($long_name) {
        if (($slash_pos = strpos($long_name, "/")) === false) {
             return $long_name;
        } else {
             return substr($long_name, 0, $slash_pos);
        }
    }

    private function convertPlanningCenterDate ($zulu) {
       // convert zulu time to local time
       $ts = strtotime($zulu);
       $local = date("Y-m-d H:i:s", $ts);
       return $local ;
    }

    private function planningCenterAPI($config, $url) {
	$curl_session = curl_init($url);
	curl_setopt($curl_session, CURLOPT_USERNAME, $config->planningCenterId);
	curl_setopt($curl_session, CURLOPT_PASSWORD, $config->planningCenterSecret);
	curl_setopt($curl_session, CURLOPT_RETURNTRANSFER, true);
	$response = curl_exec($curl_session);
	return json_decode($response, true);
    }

    private function getResourceName($config, $resourceId) {
	$resource_url = "https://api.planningcenteronline.com/calendar/v2/resources/$resourceId";
	$jres = $this->planningCenterAPI($config, $resource_url);

	return $jres["data"]["attributes"]["name"];
    }

    private function getEventName($config, $eventId) {
	$event_url = "https://api.planningcenteronline.com/calendar/v2/events/$eventId";
	$jres = $this->planningCenterAPI($config, $event_url);

	return trim($jres["data"]["attributes"]["name"]);
    }

    public function getIndex() {
        // The number returned here for this plugin must be unique to this wall-ink-server
        return 8;
    } 
    public function getName() {
        // This is the name of the plugin as it will appear in the device manager
        return "Planning Center";
    } 
    public function isActive($config) {
        // Used to see if the plugin is active
        return $config->planningCenterActive;
    }
    public function getResources($config) {
        // Build array of schedualable resources
        // The resourceID in the array do not need to be unique to each plugin, and may be any number or string VARCHAR(64)
	$resource_url = "https://api.planningcenteronline.com/calendar/v2/resources?filter=rooms&per_page=50";
	$jres = $this->planningCenterAPI($config, $resource_url);
	$rooms = array();
	foreach ($jres["data"] as $element) {
		$resource = $element["id"];
		$name = $element["attributes"]["name"];
		$rooms[$resource]=$name;
	}
        return $rooms;
    }
    private function getSchedule($config, $resourceId) {

        // To assist in debugging the getSchedule command, look in your web directory in the 
        // image_data folder.  Look for a file named <mac_address>.info which will contain the
        // text schedule for your resource.

        // Usually this function would be getting today's event info from a database or API.
        // Since we are creating dates with today's date on them to display, we need to get today's
        // date and format properly for the image generator.	
        $formatted_date = trim(`date +%Y-%m-%d`);
	$timestamp = time();
	date_default_timezone_set('America/Denver'); // Make local timezone default for PHP

        // calculate UTC time for this morning and tomorrow night
        //
        $this_morning_str = date("Y-m-d 00:00:00", $timestamp);
        $tomorrow_night_str = date("Y-m-d 23:59:59", $timestamp + 60 * 60 * 24);
        $tm = date_parse($this_morning_str);
        $tn = date_parse($tomorrow_night_str);
        $this_morning_ts = mktime(0, 0, 0, $tm["month"], $tm["day"], $tm["year"]);
        $tomorrow_night_ts = mktime(23, 59, 59, $tn["month"], $tn["day"], $tn["year"]);
        $this_morning_zulu = gmdate("Y-m-d\TH:i:s\Z", $this_morning_ts);
        $tomorrow_night_zulu = gmdate("Y-m-d\TH:i:s\Z", $tomorrow_night_ts);

	$schedule_url = "https://api.planningcenteronline.com/calendar/v2/resources/$resourceId/resource_bookings";
	$schedule_url .= "?order=starts_at";
	$schedule_url .= "&where[starts_at][gte]=$this_morning_zulu";
	$schedule_url .= "&where[starts_at][lte]=$tomorrow_night_zulu";
	$jres = $this->planningCenterAPI($config, $schedule_url);

	$room_name = $this->trimResourceName($this->getResourceName($config, $resourceId));
        $found_elaine = false;
	// go thru events and look for "Elaine"
	foreach($jres["data"] as $item) {
	    $event_id = $item["relationships"]["event"]["data"]["id"];
	    $ename = $this->getEventName($config, $event_id);
	    if (str_contains($ename, "Elaine")) {
	       $start_ts = strtotime($this->convertPlanningCenterDate($item["attributes"]["starts_at"]));
	       $end_ts = strtotime($this->convertPlanningCenterDate($item["attributes"]["ends_at"]));

               if (($start_ts <= $timestamp) && ($end_ts >= $timestamp)) {
                  $room_name = "Rev. Elaine's Office";
                  $found_elaine = true;
               }
            }
        }

        // Add the name of the resource to the top of the schedule
        $schedule = $room_name;
        $schedule .="\n";

        if ($found_elaine) {
           $schedule .="+no_events+";
           $schedule .="\n";
        }

        $today = date("Y-m-d", $timestamp);
        $tomorrow = date("Y-m-d", $timestamp + 24 * 60 * 60);

	foreach($jres["data"] as $item) {
	    $start = $this->convertPlanningCenterDate($item["attributes"]["starts_at"]);
	    $start_ts = strtotime($start);

            // only process events for today or tomorrow in local time zone
            if (!str_contains($start, $today) && !str_contains($start, $tomorrow)) {
                continue;
            }

	    $end = $this->convertPlanningCenterDate($item["attributes"]["ends_at"]);
	    // look up meeting name
	    $event_id = $item["relationships"]["event"]["data"]["id"];
	    $name = $this->getEventName($config, $event_id);

            $schedule .= $name;
            $schedule .= "\n";
            $schedule .= $start;
            $schedule .= "\n";
            $schedule .= $end;
            $schedule .= "\n";
	}

        return $schedule;
    }

    public function getImage($config, $device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetImage.php");
        //return schedulingGetImage($config, $device, $this->getSchedule($config, $device["resource_id"]), $config->exampleSchedulerDisplayUrl, $config->exampleSchedulerQrCodeBaseUrlBeginning . $device["resource_id"] . $config->exampleSchedulerQrCodeBaseUrlEnd);
        return schedulingGetImage($config, $device, $this->getSchedule($config, $device["resource_id"]), $config->planningCenterDisplayUrl, $config->planningCenterQrCodeBaseUrlBeginning);
    }

    public function getDeviceType($device) {
        require_once("$_SERVER[DOCUMENT_ROOT]/plugin_dependencies/general_scheduling/schedulingGetDeviceType.php");
        return schedulingGetDeviceType($device, $this->getIndex());
    }
}

if ($config->planningCenterActive == "true") {
    $planningCenter = new planningCenterPlugin;
    $plugins[ $planningCenter->getIndex() ] = $planningCenter;
}
?>

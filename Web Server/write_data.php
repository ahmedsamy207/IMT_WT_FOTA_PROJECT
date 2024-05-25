<?php
$file = "data.txt";
$value = $_POST["value"] ?? ""; // Get the value from the AJAX request

if ($value !== "") {
    if (file_put_contents($file, $value) !== false) {
        echo "Mission being processed.....";
    } else {
        echo "Mission Failed.";
    }
} else {
    echo "Invalid value provided.";
}
?>
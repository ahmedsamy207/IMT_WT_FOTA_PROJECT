<?php
$file = "data.txt";

// Read the content of the file
$content = file_get_contents($file);

// Erase the content of the file
file_put_contents($file, "");

echo $content;
?>
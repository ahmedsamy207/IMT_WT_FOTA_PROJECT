<?php
if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["fileToUpload"])) {
    $target_dir = "uploads/";
    $target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
    $uploadOk = 1;
    $imageFileType = strtolower(pathinfo($target_file, PATHINFO_EXTENSION));

    // Remove the existing file if it exists
    if (file_exists($target_file)) {
        unlink($target_file);
    }

    // Check file size
    if ($_FILES["fileToUpload"]["size"] > 500000) {
        $uploadOk = 0;
    }

    // Allow only HEX file format
    if ($imageFileType != "hex") {
        $uploadOk = 0;
    }

    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
        $message = "Sorry, there was an error uploading your file.";
    } else {
        if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
            $message = "Upload complete!";
        } else {
            $message = "Sorry, there was an error uploading your file.";
        }
    }
    header("Location: index.html?message=" . urlencode($message));
    exit();
}
?>

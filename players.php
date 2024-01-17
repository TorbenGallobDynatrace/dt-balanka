<?php
// Define the URL of your Supabase endpoint
$supabaseUrl = 'https://aoofsytdvtzbwkxfwnuh.supabase.co/rest/v1/players?select=*';
$anonKey = 'eyJhbGciOiJIUz...';

// Check if there is a POST request
if ($_SERVER['REQUEST_METHOD'] === 'GET') {
    // Initialize cURL session
    $ch = curl_init($supabaseUrl);

    // Set cURL options
    curl_setopt($ch, CURLOPT_HTTPHEADER, array(
        'apikey: '.$anonKey,
        'Authorization: Bearer '.$anonKey,
    ));
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, true); // Verify the peer's SSL certificate

    // Execute cURL session and get the response
    $response = curl_exec($ch);

    // Check for cURL errors
    if (curl_errno($ch)) {
        // Handle error
        echo 'cURL Error: ' . curl_error($ch);
    } else {
        // Output the response from Supabase
        echo $response;
    }

    // Close cURL session
    curl_close($ch);
} else {
    echo "This script only handles GET requests.";
}
?>
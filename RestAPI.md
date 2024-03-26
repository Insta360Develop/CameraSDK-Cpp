# REST API Documentation
## Overview
This documentation outlines the RESTful endpoints provided by our camera control server. These endpoints allow for interactions such as querying camera status, taking photos, managing recordings, and retrieving file lists.

## Base URL
The base URL for the API is: http://localhost:**9091**

## Endpoints
### Health Check
- **URL**: /api/v1/healthy
- **Method**: GET
- **Description**: Checks if the server is running and responsive.
- Request Body: None
- **Success Response**:
    - Code: 200 OK
    - Content: "Healthy"
### Get Serial Numbers
- **URL**: /api/v1/serialNumbers
- **Method**: GET
- Query Parameters:
    - index (optional): Index of the camera from which to retrieve serial numbers. Defaults to 0 if not provided.
- **Description**: Retrieves the serial numbers of all cameras.
- Request Body: None
- **Success Response**:
    - Code: 200 OK
    - Content: A JSON object with the following fields:
        - "status": A string indicating the request status ("success").
        - "serialNumbers": An array of strings, each representing a serial number of the camera(s).
- **Error Response**:
    - Code: 400 Bad Request
    - Content: A JSON object with the following fields:
        - "status": A string indicating the request status ("error").
        - "message": A string detailing the error encountered.
### Take Photo
- **URL**: /api/v1/takePhoto
- **Method**: POST
- **Required Parameters**:
    - cameraIndex: Index of the camera to use for taking the photo.
- **Description**: Initiates taking a photo using the specified camera.
- **Success Response**:
    - Code: 200 OK
    - Content: A JSON object with the following fields:
        - "status": A string indicating the request status ("success").
        - "message": A string with a confirmation message ("Photo taken successfully").
        - "deviceUrl": A string representing the URL to the photo on the device.
        - "homeUrl": A string representing the local filesystem path where the photo was saved.
        - "wwwUrl": A string representing a publicly accessible URL to the photo (if applicable).
- **Error Response**:
    - Code: 400 Bad Request or 500 Internal Server Error (for server-side issues, such as failure to take a photo).
    - Content: A JSON object with the following fields:
        - "status": A string indicating the request status ("error").
        - "message": A string detailing the error encountered.
### Get File Lists
- **URL**: /api/v1/getFileLists
- **Method**: POST
- **Required Parameters**:
    - cameraIndex: Index of the camera from which to retrieve the file list.
- **Description**: Retrieves a list of files available from the specified camera.
- **Success Response**:
    - Code: 200 OK
    - Content: JSON object with a list of files.
        - "status": A string indicating the request status ("error").
        - "files": An array of strings, each representing a path to file in camera. Might be passed to download a file
- **Error Response**:
    - Code: 400 Bad Request
    - Content: JSON object with error details.
        - "status": A string indicating the request status ("error").
        - "message": A string detailing the error encountered.
### Start Recording
- **URL**: /api/v1/startRecording
- **Method**: POST
- **Required Parameters**:
    - cameraIndex: Index of the camera to start recording.
- **Description**: Starts recording on the specified camera.
- **Success Response**:
    - Code: 200 OK
    - Content: JSON object indicating success.
        - "status": A string indicating the request status ("success").
        - "message": A string with a confirmation message ("Recording started successfully").
- **Error Response**:
    - Code: 400 Bad Request
    - Content: JSON object with error details.
        - "status": A string indicating the request status ("error").
        - "message": A string detailing the error encountered, such as "Invalid camera index" or "Failed to start recording."
### Stop Recording
- **URL**: /api/v1/stopRecording
- **Method**: POST
- **Required Parameters**:
    - cameraIndex: Index of the camera to stop recording.
- **Description**: Stops recording on the specified camera and downloads the recordings.
- **Success Response**:
    - Code: 200 OK
    - Content: JSON object including the recording URLs and success message.
        - "status": A string indicating the request status ("success").
        - "message": A string confirming that recording was stopped and files were downloaded ("Recording stopped and files downloaded successfully").
        - "deviceUrls": An array of strings, each a URL to a recording on the device.
        - "homeUrls": An array of strings, each a local filesystem path where a recording was saved.
        - "wwwUrls": An array of strings, each a publicly accessible URL to a recording (if applicable).
- **Error Response**:
    - Code: 400 Bad Request
    - Content: JSON object with error details.
        - "status": A string indicating the request status ("error").
        - "message": A string detailing the error encountered, such as "Invalid camera index" or "Failed to stop recording."
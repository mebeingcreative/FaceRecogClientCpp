# FaceRecogClientCpp

## Requirements

Tested on Ubuntu 16.04 and 17.04

* Webcam
* OpenCV 2.4
* Qt5
* LAPACK
* [Face Recog Python Services](https://github.com/firemind/face-recog-server-python)

## Install

### Build from Source

Please see [FaceRecogClientCppDocker]() for an semi automated build enviroment.


### Install the deb

You need to build the project and run cpack to create the debs.

```bash
apt-get install libopencv-dev, qtbase5-dev, libqt5webkit5-dev, libopenblas-dev, liblapack-dev
dpkg -i face-recog-client.deb
```

### Configure & Run

The embedding service **see Face Recog Python Services** must run and the tracking service **see Face Recog Server**
must be available.

The `/usr/local/etc/face-recog-config.ini` config file needs to be updated:

* *embedding_service_url* **= "http://localhost:5001/"**
  * Preferably on the same machine for better performance and network delay.
* *tracking_api_url* **= "https://face.otep.ch/"**
  * The single endpoint for all camera instances to send camera images and embeddings to.
* *location_name*  **= "cam2"**
  * Should be unique across all camera instances.
* *tracking_view_url* **= "https://face.otep.ch/locations/$location_name/current"**
  * Website with recognized faces. Should be the same server that handles the tracking api requests.
* *predictor_path* **= "${FACE_RECOG_LANDMARK_PATH}"**
  * Landmark file with the predictor shapes for facial detection.

## Architecture

![Diagram](https://github.com/viruch/FaceRecogClientCpp/raw/master/figures/face_recog_full.png "Architecture")


## Documentation

![Wiki](https://github.com/firemind/facial-recognition-sa-2017/wiki)

## Acknowledgements

Faces are detected with dlib's frontal face detector (http://dlib.net/)
Face embeddings are calculated using the Tensorflow-based facenet (https://github.com/davidsandberg/facenet)

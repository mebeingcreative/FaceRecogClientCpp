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

## Architecture

![Diagram](https://github.com/viruch/FaceRecogClientCpp/raw/master/figures/face_recog_full.png "Architecture")


## Documentation

![Wiki](https://github.com/firemind/facial-recognition-sa-2017/wiki)

## Acknowledgements

Faces are detected with dlib's frontal face detector (http://dlib.net/)
Face embeddings are calculated using the Tensorflow-based facenet (https://github.com/davidsandberg/facenet)

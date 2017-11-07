// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    

    This example is essentially just a version of the face_landmark_detection_ex.cpp
    example modified to use OpenCV's VideoCapture object to read from a camera instead 
    of files.


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/
#include <vector>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include "clock.h"

cv::Rect dlibRectangleToOpenCV(dlib::rectangle const & r) {
  return {cv::Point2i(r.left(), r.top()), cv::Point2i(r.right() + 1, r.bottom() + 1)};
}

void convert_to_jpeg(cv::Mat & mat, std::vector<uchar> & out) {
    std::vector<int> params{ cv::IMWRITE_JPEG_QUALITY, 80 };
    cv::imencode(".jpg", mat, out, params);
}

template<int margins = 44L>
void grow_margin(dlib::rectangle & r) {
    r.left() -= margins;
    r.top() -= margins;
    r.right() += 2 * margins;
    r.bottom() += 2 * margins;
}

int main() {
    try {
        cv::VideoCapture cap{0};
        if (!cap.isOpened()) {
            std::cerr << "Unable to connect to camera\n";
            return 1;
        }

        dlib::image_window win;
        dlib::image_window face_win;

        // Load face detection and pose estimation models.
        auto detector = dlib::get_frontal_face_detector();
        dlib::shape_predictor pose_model;
        dlib::deserialize("../res/shape_predictor_68_face_landmarks.dat") >> pose_model;

        auto clock = speed_clock{std::cout};

        // Grab and process frames until the main window is closed by the user.
        while (!win.is_closed() && !face_win.is_closed()) {
            // Grab a frame
            cv::Mat originalImage;
            if (!cap.read(originalImage)) {
                break;
            }

            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            dlib::cv_image<dlib::bgr_pixel> cimg(originalImage);


            // Detect faces 
            std::vector<dlib::rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<dlib::full_object_detection> shapes;
            for (auto & face : faces) {
                shapes.push_back(pose_model(cimg, face));
//                cv::Mat face_mat;
//                originalImage.copyTo(face_mat(dlibRectangleToOpenCV(face)));

                grow_margin(face);
            }

            // Display it all on the screen
            win.clear_overlay();
            win.set_image(cimg);
            win.add_overlay(render_face_detections(shapes));

            dlib::array<dlib::array2d<dlib::rgb_pixel>> face_chips;
            extract_image_chips(cimg, get_face_chip_details(shapes), face_chips);
            face_win.set_image(dlib::tile_images(face_chips));

            clock.step();
        }
    }
    catch (dlib::serialization_error & e) {
        std::cout << "You need dlib's default face landmarking model file to run this example.\n"
                          "You can get it from the following URL: \n"
                          "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2\n\n"
                  << e.what() << "\n";
    }
    catch (std::exception & e) {
        std::cout << e.what() << "\n";
    }
}


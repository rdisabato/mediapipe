#include <cmath>
#include <functional>
#include <vector>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/ret_check.h"
#include <glog/logging.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 

namespace {
    constexpr char kFaceLandmarksTag[] = "FACE_LANDMARKS";
}

namespace mediapipe {
    class DataSenderHolisticCalculator: public CalculatorBase {
        public:

        static ::mediapipe::Status GetContract(CalculatorContract* cc) {
            LOG(INFO) << "DataSenderHolisticCalculator: GetContract() called";
            if (cc->Inputs().HasTag(kFaceLandmarksTag)) {
                LOG(INFO) << "DataSenderHolisticCalculator: Detected face landmarks";
                cc->Inputs().Tag(kFaceLandmarksTag).Set<NormalizedLandmarkList>();
            }
            
            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Open(CalculatorContext* cc) {
            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Process(CalculatorContext* cc) {

            NormalizedLandmarkList faceLandmarks = cc->Inputs().Tag(kFaceLandmarksTag).Get<NormalizedLandmarkList>();
            std::string faceLandmarksMessage = faceLandmarks.ShortDebugString();
            
            std::string hostname = "127.0.0.1";
            uint16_t port = 33333;
            int sock = socket(AF_INET, SOCK_DGRAM, 0);
            sockaddr_in destination;
            destination.sin_family = AF_INET;
            destination.sin_port = htons(port);
            destination.sin_addr.s_addr = inet_addr(hostname.c_str());
            int n_bytes = ::sendto(sock, faceLandmarksMessage.c_str(), faceLandmarksMessage.length(), 0, reinterpret_cast<sockaddr*>(&destination), sizeof(destination));
            ::close(sock);

            return ::mediapipe::OkStatus();
        }

        ::mediapipe::Status Close(CalculatorContext* cc) {
            return ::mediapipe::OkStatus();
        }



    };

    REGISTER_CALCULATOR(DataSenderHolisticCalculator);
    
}   
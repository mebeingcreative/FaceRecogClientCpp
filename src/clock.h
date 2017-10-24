//
// Created by viruch on 24.10.17.
//

#ifndef FACERECOGCLIENTCPP_CLOCK_H
#define FACERECOGCLIENTCPP_CLOCK_H

#include <chrono>
#include <ostream>

struct speed_clock{
    using clock_type = std::chrono::high_resolution_clock;
    using time_point = clock_type::time_point;

    speed_clock(std::ostream & out):
            next{clock_type::now() + interval},
            count{1},
            out{out}
    {}

    void step() {
        if (next < clock_type::now()){
            out << (std::chrono::duration_cast<std::chrono::milliseconds>(interval) / count).count()
                << "ms per frame\n";
            count = 1;
            next = clock_type::now() + interval;
        } else {
            ++count;
        }
    }

private:
    std::chrono::seconds const interval{5};
    time_point next;
    long count;
    std::ostream & out;
};

#endif //FACERECOGCLIENTCPP_CLOCK_H

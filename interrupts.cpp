/**
 *
 * @file interrupts.cpp
 * @author Sasisekhar Govind
 *
 */

#include "interrupts.hpp"
#include<cmath>
int main(int argc, char** argv) {

    //vectors is a C++ std::vector of strings that contain the address of the ISR
    //delays  is a C++ std::vector of ints that contain the delays of each device
    //the index of these elemens is the device number, starting from 0
    auto [vectors, delays] = parse_args(argc, argv);
    std::ifstream input_file(argv[1]);

    std::string trace;      //!< string to store single line of trace file
    std::string execution;  //!< string to accumulate the execution output

    /******************ADD YOUR VARIABLES HERE*************************/
    int time = 0;
    int device_number = 0;
    int num_activities = 0;

    /******************************************************************/

    //parse each line of the input trace file
    while(std::getline(input_file, trace)) {
        if (trace.empty()) continue;
            auto [activity, duration_intr] = parse_trace(trace);

            /******************ADD YOUR SIMULATION CODE HERE*************************/
        if (activity == "END_IO") {
            int device_number = duration_intr;
            int duration = delays[device_number];
            std::string isr_address = vectors[device_number];

            execution += std::to_string(time) + ", 1, switch to kernel mode\n";
            execution += std::to_string(time+1) + ", 10, context saved\n";
            execution += std::to_string(time+11) + ", 1, find vector " + std::to_string(device_number+1) 
            + " in memory position " + isr_address + "\n";
            execution += std::to_string(time+12) + ", 1, obtain ISR address\n";
            execution += std::to_string(time+13) + ", 24, call device driver\n";

            int remaining = duration;
            int current_time = time + 13;
            int activity_num = 1;

            while (remaining > 0) {
                int block = std::min(40, remaining);
                execution += std::to_string(current_time) + ", " + std::to_string(block)
                            + ", ISR activity " + std::to_string(activity_num)
                            + " with device number: " + std::to_string(device_number + 1) + "\n";
                current_time += block;
                remaining -= block;
                activity_num++;
            }

            execution += std::to_string(current_time) + ", 1, IRET\n";
            execution += std::to_string(current_time + 1) + ", " + std::to_string(duration)
                        + ", end of I/O " + std::to_string(device_number + 1) + ": interrupt\n";

            time = current_time + duration;
        }
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

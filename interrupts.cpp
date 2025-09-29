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

            num_activities = ceil(duration / 40);

            execution += std::to_string(time) + ", 1, switch to kernel mode\n";
            execution += std::to_string(time+1) + ", 10, context saved\n";
            execution += std::to_string(time+11) + ", 1, find vector " + std::to_string(device_number+1) 
            + " in memory position " + isr_address + "\n";
            execution += std::to_string(time+12) + ", 1, obtain ISR adddress\n";
            execution += std::to_string(time+13) + ", 24, call device driver\n";

            for (int i = 0; i < num_activities; i++) {
                execution += std::to_string(time+13 + i*40) + ", 40, ISR activity " 
                + std::to_string(i+1) + " with device number: " + std::to_string(device_number + 1) + "\n";
            }

            execution += std::to_string(time+13 + num_activities*40) + ", 1, IRET\n";
            execution += std::to_string(time+13 + num_activities*40 + 1) + ", " + std::to_string(duration)
            + ", end of I/O " +std::to_string(device_number + 1) + ": interrupt\n";
        }
        else {
            execution += std::to_string(time) + ", " + std::to_string(duration_intr) + ", " + activity + "\n";
            time += duration_intr;
        }
        /************************************************************************/

    }

    input_file.close();

    write_output(execution);

    return 0;
}

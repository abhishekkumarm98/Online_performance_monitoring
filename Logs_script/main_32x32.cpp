/*
 * Copyright (c) 2013 Regents of the University of California. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. The names of its contributors may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * *********************************************************************************************** *
 * CARLsim
 * created by:      (MDR) Micah Richert, (JN) Jayram M. Nageswaran
 * maintained by:   (MA) Mike Avery <averym@uci.edu>, (MB) Michael Beyeler <mbeyeler@uci.edu>,
 *                  (KDC) Kristofor Carlson <kdcarlso@uci.edu>
 *
 * CARLsim available from http://socsci.uci.edu/~jkrichma/CARL/CARLsim
 */
#include <carlsim.h>
#include <visual_stimulus.h>

#include <vector>
#include <cassert>
#include <string>
#include <cstdio>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <sys/stat.h>

using namespace std;


int main(int argc, const char* argv[]) {

int nG1 = 32; // #Pre-synaptic neurons  
int nG2 = 32; //#Post-synaptic neurons
int iterations=10000;

    // ---------------- CONFIG STATE -------------------
    CARLsim sim("out", GPU_MODE, USER,0, -1+43);

    // Input stimulus created from an image using the MATLAB script
    // "createStimFromImage.m":
    // VisualStimulus stim("input/temp.dat");
    // stim.print();

    // Arrange neurons on a 3D grid, such that every neuron corresponds to
    // pixel: <width x height x channels>. For a grayscale image, the number
    // of channels is 1 -- for an RGB image the number is 3.
    // Grid3D imgDim(stim.getWidth(), stim.getHeight(), stim.getChannels());

    // The output group should be smaller than the input, depending on the
    // Gaussian kernel. The number of channels here should be 1, since we
    // will be summing over all color channels.
    // Grid3D imgSmallDim(imgDim.numX/2, imgDim.numY/2, 1); 

    int gIn0 = sim.createSpikeGeneratorGroup("input", nG1, EXCITATORY_NEURON);
    int gOut = sim.createGroup("out", nG2, EXCITATORY_NEURON);

    sim.setNeuronParameters(gOut, 0.02f, 0.2f, -65.0f, 550.0f); // a,b,c,d

    int connId = sim.connect(gIn0, gOut, "full", RangeWeight(5.0f),1.0f);

    // Use CUBA mode
    sim.setConductances(true);
    
    // ---------------- SETUP STATE -------------------

    sim.setupNetwork();
    float w = 1.0f;
    SpikeMonitor* in0 = sim.setSpikeMonitor(gIn0, "DEFAULT");
    SpikeMonitor* out = sim.setSpikeMonitor(gOut, "DEFAULT");


    const int dir_err = mkdir("Output",  S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

    if (dir_err == -1){
    	printf("Error creating directory !");
    	exit(1);
	}


    ConnectionMonitor* cm0 = sim.setConnectionMonitor(gIn0, gOut, "DEFAULT");
    for(int col=0; col<nG1; col++)
    {

      ofstream file;

      file.open("Output/log_weight" +to_string(col)+".txt");

      float var2 = 0.001;
      for(int postNeuron_id=0; postNeuron_id<nG2; postNeuron_id++){

        
        for (int preNeuron_id=0;preNeuron_id<nG1; preNeuron_id++)
        {
            
            float var = (1.0f/(10.0f*(1.0f+preNeuron_id)));

            if (preNeuron_id <= col)
            {
                // Connection
                sim.setWeight(connId, preNeuron_id, postNeuron_id, w-var-var2);
                // var2 += 0.00000000001;
                var2 += 0.0009;
            }
            else
            {   
                sim.setWeight(connId, preNeuron_id, postNeuron_id, 0.0f);
            }   
        }

        }
        
        std::vector<std::vector<int>> spikeMat;

        std::vector<std::vector<int>> spikeMat_out;
        for (int i=0; i<iterations; i++) 
        {

            in0->startRecording();
            out->startRecording();
            // ---------------- RUN STATE -------------------
            PoissonRate poissRate0(nG1,false);
            poissRate0.setRates(10.0f);
            
            sim.setSpikeRate(gIn0, &poissRate0);
            sim.runNetwork(1,0); // run the network 1 sec
            

            in0->stopRecording();
            out->stopRecording();

            //in0->print();
            //out->print();
            //cm0->printSparse(ALL);
           
            vector<float> spikeRates =  in0->getAllFiringRates();
            vector<float>::iterator iter;

            //printf("Column number: %d\n", col);
            file << "Time : ";
            file << i + 1;
            file << " sec\n";
            // file << "Column number: ";
            // file << col;
            // file << "\n";
           

            //printf("\n");
            // print the column information to a text file 
            spikeMat = in0->getSpikeVector2D();
            vector<vector<int>>::iterator it;
            int rowid = 0;

            // For output
            spikeMat_out = out->getSpikeVector2D();
            vector<vector<int>>::iterator it_out;

            // print the column number
            // print the spike times for each neuron in a column
            for(it=spikeMat.begin(); it!=spikeMat.end(); it++)
            {
                vector<int>::iterator spike;
               // printf("NeuronID: %d Spike Rate: %f Spike Times: ", rowid);
                
                file << "NeuronID: ";
                file << rowid;
                file << ", Spike Times: ";
                
                   

                for(spike=it->begin(); spike!=it->end(); spike++)
                {
                 //   printf("%d ", (*spike));
                    file << *spike;
                    file << " ";                    
                }
                rowid++;

               file << "\n";
               //printf("\n \n");
            }
            
            
            // print the spike times for output neuron in a column
            
            int column_id = 0;
            for(it_out=spikeMat_out.begin(); it_out!=spikeMat_out.end(); it_out++)
            {
                vector<int>::iterator spike_out;

                //printf("Output Neuron at column %d and its spike time: ", col);
                file << "Output Neuron at column ";
                file << column_id;
                file << " and its spike time: ";
                column_id++;
                        
                for(spike_out=it_out->begin(); spike_out!=it_out->end(); spike_out++)
                {
                 //   printf("%d ", (*spike_out));
                    file << *spike_out;
                    file << " ";
                }
                
               // printf("\n \n");

                file << "\n";
            }

        file << "\n";

        }

        file.close();
    

    }

return 0;
}


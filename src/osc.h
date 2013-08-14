//
//  osc.h
//  modelMapper
//
//  Created by usm on 6/7/13.
//
//

#ifndef __modelMapper__osc__
#define __modelMapper__osc__

#include <iostream>
#include "ofxOsc.h"
// OSC listen on port 12345 and 10000
#define PORT 12006
#define NUM_MSG_STRINGS 20

class OscThread : public ofThread{
    
public:
    
    //--------------------------------touchOSC
    ofxOscReceiver	receiver;
    
    float   timers[NUM_MSG_STRINGS];
    int current_msg_string;
    string  mouseButtonState;
    
    string  msg_strings[NUM_MSG_STRINGS];
    
    float mag[3] = {0.0, 0.0, 0.0};
     int count;  // threaded fucntions that share data need to use lock (mutex)
    // and unlock in order to write to that data
    // otherwise it's possible to get crashes.
    //
    // also no opengl specific stuff will work in a thread...
    // threads can't create textures, or draw stuff on the screen
    // since opengl is single thread safe
    
    //--------------------------
    OscThread(){
        count = 0;
    }
    
    void start(){
        startThread(true, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    //--------------------------
    void threadedFunction(){
    }
    
    //--------------------------
    void setup(){
        // listen on the given port
        cout << "listening for osc messages on port " << PORT << "\n";
        receiver.setup( PORT );
    }
    
    void update(){
        while( receiver.hasWaitingMessages() )
        {
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage( &m );
                        
            if( m.getAddress() == "/sound" ){
                mag[0] = m.getArgAsFloat( 0 );
                mag[1] = m.getArgAsFloat( 1 );
                mag[2] = m.getArgAsFloat( 2 );
            }else{
//                cout << "other" << endl;
            }
        }
        
    }
};
#endif /* defined(__modelMapper__osc__) */

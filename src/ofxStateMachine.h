//
//  ofxStateMachine.h
//  BaseApp
//
//  Created by Oriol Ferrer Mesià on 30/07/14.
//
//

#ifndef BaseApp_StateMachine_h
#define BaseApp_StateMachine_h

#include "ofMain.h"

#define SET_NAME_FOR_STATE( STATE ) setNameForState(STATE, #STATE)

template <class T>
class ofxStateMachine{ //finite state machine

public:

	ofxStateMachine();
	~ofxStateMachine();

/// INIT ///////////////////////////////////////////////////////////////

	void setNameForState(T state_, std::string name);
	std::string getNameForState(T state_);
	std::string getNameAndQuickStatusForState(T state_);


/// SETTING / GETTING STATE /////////////////////////////////////////////

	void setState(T newState, bool clearPreviousErr = true);
	T getState();

	std::string getCurrentStateName();
	float getElapsedTimeInCurrentState();

/// IN-STATE ERROR HANDLING / RETRY N TIMES /////////////////////////////

	//we found an error, we stay on the same state; we will retry N times in M seconds
	void setError(std::string msg, float retryAgainInSeconds, int maxRetries_);
	bool hasError(); //see if we reported an error before (in the current state)

	int getNumTimesRetried();
	int getMaxRetryTimes();

	//if this state reached the specified,
	//max number of error retries (time to give up?)
	bool ranOutOfErrorRetries();


/// FLOW CONTROL ////////////////////////////////////////////////////////

	bool isReadyToProceed(); //only useful to hold state longer after an error (ie report to screen)

/// DEBUG ////////////////////////////////////////////////////////////////

	std::string getStatusMessage();


	struct StateChangedEventArgs{
		T oldState;
		T newState;
		float timeInPrevState;
	};

	struct ErrorStateEventArgs{
		T state;
		std::string errorMsg;
		int numTimes;
	};

	ofEvent<StateChangedEventArgs> eventStateChanged;
	ofEvent<ErrorStateEventArgs> eventStateError;

protected:

	void update(ofEventArgs & args);

	T state;
	T prevState;
	bool firstState;

	float stateTime; //count time spent in this state;
	float waitTime; //used to control flow, countdown
	int numRetries;	//in case something went wrong, count how many times we retried;
	int maxRetries; //how many times to retry max before

	bool error;
	std::string errorMsg;

	std::map<T, std::string> stateNames;

	void clearErrorStatus();
	bool isSetup = false;
	float lastStateChangeTime = 0;
};


#endif


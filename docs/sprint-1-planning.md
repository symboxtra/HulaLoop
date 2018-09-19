![HulaLoop](/res/logo/hulaloop-header-small.png)

# **Sprint 1 Planning Document** ##
Team 30: Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Neel Patel 
## **Sprint Overview** ##
During this sprint, we plan to work on the essential architecture and components of our application. We aim to first setup our development environments and dependencies (Qt Creator, GitHub, Jenkins, CMake, GoogleTest, JACK Audio, and PortAudio). Once setup, our goal is to begin the first interactions with the audio APIs of each of our targeted operating systems. This goal will lead us to develop the foundation for our audio backend. The sprint will also include the first workings of our user interfaces and the interconnection between the frontend and the backend. As a part of all this work, we will develop and adhere to coding standards and documentation.

## **Scrum Master:** ##

 Jack McKernan


## **Meeting Plan:** ## 

Monday, Wednesday, Friday 5:00 PM - 7:00 PM
## **Risks and Challenges** ##
Since this is the first sprint, we will likely face many challenges, including ones that we cannot currently foresee. Some of the primary challenges that we do foresee, however, are configuring dependencies and environments for CI servers and each developer, using previously unexplored APIs and technologies, and setting coding standards and guidelines. Some of the risk that we associate with this sprint is directly linked to the challenge of using previously unexplored APIs and technologies. Since the system-level APIs and structures are complex and differ for each system, we anticipate a steep learning curve using these. As they form the basis for our application, we plan to dedicate great effort to optimally designing them. This may involve changing our initial assumptions about the structure or organization of our classes.
## **Current Sprint Detail** ##
### **User Story #1:** ###
As a developer, I would like to get an audio sample from Windows.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research and implement the WASAPI interface | 4 | Neel |
| 2 | Wrap WASAPI specific details into the Device class | 3 | Neel |
| 3 | Initialize a WASAPI endpoint as a Device | 1 | Neel |
| 4 | Handle device/WASAPI errors | 2 | Neel |
| 5 | Request and render a sample from the Device | 3 | Neel |
| 6 | Unit Test - Read from input and output devices to ensure sampling process works correctly | 1 | Neel |

#### Acceptance Criteria: ####

- Given that the WASAPI interface is implemented correctly, when a buffer of audio is available, it is stored in the OSAudio class and should contain the captured data.
- Given that device/WASAPI errors are handled, when an error occurs then the application generates an error message.
- Given that the application can successfully request samples, when a sample is received, the data is written to a temporary file and can be viewed.

### **User Story #2:** ###

As a developer, I would like to get an audio sample from Linux.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research and implement the JACK2 interface for Linux | 4 | Emanuel |
| 2 | Wrap JACK2 specific details into the Device class | 3 | Emanuel |
| 3 | Initialize JACK2 Device | 1 | Emanuel | 
| 4 | Handle device/JACK2 errors | 2 | Emanuel |
| 5 | Request and render a sample from the Device | 3 | Emanuel |
| 6 | Unit Test - Read from input and output devices to ensure sampling process works correctly | 1 | Emanuel |

#### Acceptance Criteria: ####

Given that the JACK2 interface is implemented correctly, when a buffer of audio is available, it is stored in the OSAudio class and should contain the captured data.
Given that device/JACK2 errors are handled, when an error occurs then the application generates an error message.
Given that the application can successfully request samples, when a sample is received, the data is written to a temporary file and can be viewed.

### **User Story #3:** ###

As a developer, I would like to get an audio sample from Mac OS X.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research and implement the JACK2 interface for macOS | 4 | Jack |
| 2 | Wrap JACK2 specific details into the Device class | 3 | Jack |
| 3 | Initialize JACK2 Device | 1 | Jack |
| 4 | Handle device/JACK2 errors | 2 | Jack |
| 5 | Request and render a sample from the Device | 3 | Jack |
| 6 | Unit Test - Read from input and output devices to ensure sampling process works correctly | 1 | Jack |

#### Acceptance Criteria: ####

Given that the JACK2 interface is implemented correctly, when a buffer of audio is available, it is stored in the OSAudio class and should contain the captured data.
Given that device/JACK2 errors are handled, when an error occurs then the application generates an error message.
Given that the application can successfully request samples, when a sample is received, the data is written to a temporary file and can be viewed.

### **User Story #4:** ###

As a user, I would like to view all available audio devices.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Get list of available audio devices from Windows | 2 | Neel |
| 2 | Get list of available audio devices from macOS | 2 | Jack |
| 3 | Get list of available audio devices from Linux | 2 | Emanuel |
| 4 | Store input and output device lists in OSAudio | 1 | Neel |
| 5 | Build interface between Controller and OSAudio | 1 | Neel |
| 6 | Build Device interface between Controller and GUI | 2 |Neel |
| 7 | Display Device information in GUI combo-boxes | 1 | Emanuel |
| 8 | Unit Test - Verify the interface between Controller and OSAudio | 1 | Emanuel |
| 9 | Unit Test - Verify the interface between Controller and user interfaces | 1 | Emanuel |

#### Acceptance Criteria: ####

Given that retrieval of audio devices works, a correct list of input devices should be present in OSAudio.
Given that retrieval of audio devices works, a correct list of output devices should be present in OSAudio.
Given that the correct list of input devices are in the OSAudio list, a user should be able to see them in the GUI.
Given that the correct list of output devices are in the OSAudio list, a user should be able to see them in the GUI.
Given that the interface and controller are correctly bridged, our unit tests should pass.
Given that the GUI displays the available devices, a confirmation of change can be displayed when a user changes the selected device.

### **User Story #5:** ###
As a developer, I would like the build system to be modular and support cross-platform compilation.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Design CMake build script | 6 (3 per person) | Jack & Neel |
| 3 | Build and customize additional libraries (GoogleTest, Qt, JACK Audio, PortAudio) | 4 (1 per lib) | Jack & Neel |
| 4 | Configure and organize modules to compile as static or dynamic libraries (.so/.a, .lib/.dll) and link into the final application | 2 | Aneesh |
| 5 | Add CMake flags to allow compiling only certain modules or configurations of the application | 2 | Aneesh |
| 6 | Experiment with CPack to create a basic installer | 3 | Aneesh |

#### Acceptance Criteria: ####

Given that the modular build system works, a developer should successfully be able to compile the application and generate the necessary library files across all supportable platforms (Windows, OSX, Linux).
Given that the additional libraries have been integrated, when the application is compiled, any required dependencies are also compiled.
Given that the build system is modular, when a third-party developer wants to compile only the backend, a specific subset of the application is compiled.
Given that the build process completes, a user should have access to generated binaries and packaged installers.
Given that the build process completes, the generated binaries and executables should be in the bin folder.

### **User Story #6:** ###

As a developer, I would like to setup a continuous integration (CI) process so that each repository push triggers a build and runs tests.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create Jenkins pipelines for Windows, Linux, and macOS | 3 | Jack |
| 2 | Configure dependencies on each server | 2 | Jack |
| 3 | Create build and test scripts in the scripting language of each OS | 3 | Aneesh |
| 4 | Generate code coverage reports after test suite completion and upload to codecov.io | 1 | Aneesh |

#### Acceptance Criteria: ####

Given that the Jenkins pipeline is stable, the Jenkins server has to have the necessary dependencies (e.g. Qt framework, JACK2 API, etc.) downloaded and configured properly in order to build and run the code.
Given that the code compiles without errors, the CI process correctly triggers a build and runs the specified unit tests to ensure the code is working as intended.
Given the test suites finish without errors, a code coverage report should be generated and uploaded to codecov.io.

### **User Story #7:** ###
As a user, I would like to have a GUI.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Implement bridge between C++ and QML | 2 | Aneesh |
| 2 | Layout the GUI components using a layout manager | 2 | Aneesh |
| 3 | Design color scheme and styles | 1 | Aneesh |
| 4 | Implement color scheme and styles | 1 |Aneesh |

#### Acceptance Criteria: ####

Given that the bridge between C++ and QML is implemented, when we send data from the backend to the GUI then the GUI should be updated.
Given that the bridge between C++ and QML is implemented, when we send signals from the GUI to the backend then the specified actions should be performed.
Given that the GUI is laid out using a layout manager, when the GUI is resized, then the elements should resize and remain usable.
Given that the color scheme and styles are designed, when we add components to the GUI, they should be styled accordingly.

### **User Story #8:** ###

As a user, I would like to have a button to start audio recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled start button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 |Aneesh |

#### Acceptance Criteria: ####

Given that the record button is styled, the button’s design matches the theme of the rest of the GUI.
Given that the record button is integrated, the backend can receive click events and respond accordingly.
Given that the record button is integrated, the user should receive visual feedback when the button is pressed.

### **User Story #9:** ###

As a user, I would like to have a button to stop audio recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled stop button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |

#### Acceptance Criteria: ####

Given that the stop button is styled, the button’s design matches the theme of the rest of the GUI.
Given that the stop button is integrated, the backend can receive click events and respond accordingly.
Given that the stop button is integrated, the user should receive visual feedback when the button is pressed.

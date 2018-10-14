![HulaLoop](/res/logo/hulaloop-header-small.png)

# **Sprint 1 Retrospective** #
Team 30: Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Meriem Bounab, Neel Patel

## **What Went Well?** ##
During this sprint, we managed to complete the major backbone and basic, functional user interface of our application. Most of the OS-specific APIs and code for capturing audio has been successfully abstracted for our targeted platforms. The modular class structure and build system that we planned during our design phase turned out extremely well. There was very little repetition of code, and building cross-platform is seamless. Our test suite, code quality/standards, and CI processes also merged smoothly with our workflow. We brought on an additional team member part-way through the sprint. Despite the change, we were able to assimilate her into the team and work to find suitable tasks for this sprint.

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

#### Completed: ####
The implemented WASAPI interface can successfully capture and serve raw audio. The Windows device specific details were wrapped into the Device class. The application detects errors caused by WASAPI and reports it to the user, currently via the debug console until further error handling efforts bring these to the foreground.

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

#### Completed: ####
We initially planned to use JACK for this task, but switched to ALSA since it is installed by default on most major distros and is perfect for our needs. ALSA was also much lighter than JACK, so we decided to make the unplanned switch. The Linux/ALSA specific details were wrapped into the device class. Linux sound cards can be initialized, and raw audio data can be printed or saved. If no audio devices are present or devices errors occur, our application detects them and attempts to recover.

### **User Story #3:** ###
As a developer, I would like to get an audio sample from Mac OS X.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research and implement the JACK2 interface for macOS | 4 | Jack |
| 2 | Wrap JACK2 specific details into the Device class | 3 | Jack |
| 3 | Initialize JACK2 Device | 1 | Jack |
| 4 | Handle device/JACK2 errors | 2 | Jack |
| 5 | Request and render a sample from the Device | 3 | Jack |

#### Completed: ####
This task ended up being quite more burdensome than expected. On macOS High Sierra, the JACK2 implementation of JackRouter (JACK’s audio input device) was non-functional. This forced us to work out our own HAL audio driver in place of JackRouter. We were able to interface the driver with JACK and maintain our vision of a JACK powered system. Device/JACK errors are detected and printed to the debug console until further error handling efforts bring these to the foreground. Requesting and rendering data from the driver via JACK was successful.

### **User Story #4:** ###
As a user, I would like to view all available audio devices.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Get list of available audio devices from Windows | 2 | Neel |
| 2 | Get list of available audio devices from macOS | 2 | Jack |
| 3 | Get list of available audio devices from Linux | 2 | Emanuel |
| 4 | Store input and output device lists in OSAudio | 1 | Neel |
| 5 | Build interface between Controller and OSAudio | 1 | Neel |
| 6 | Build Device interface between Controller and GUI | 2 | Neel |
| 7 | Display Device information in GUI combo-boxes | 1 | Emanuel |
| 8 | Unit Test - Verify the interface between Controller and OSAudio | 1 | Emanuel |
| 9 | Unit Test - Verify the interface between Controller and user interfaces | 1 | Emanuel |

#### Completed: ####
We can retrieve and display the connected audio devices on all targeted operating systems (Windows, Linux, OSX). We built a Controller class that acts as a bridge between the UI layer and the OS layer. The Controller asynchronously relays the captured audio data and the device list to the upper UI layers of our architecture. This interface between the layers is an integral part of our application therefore it was necessary for us to thoroughly test these connections.

#### User Story #5: ####
As a developer, I would like the build system to be modular and support cross-platform compilation.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Design CMake build script | 6 (3 per person) | Jack & Neel |
| 2 | Build and customize additional libraries (GoogleTest, Qt, JACK Audio, PortAudio) | 4 (1 per lib) | Jack & Neel |
| 3 | Configure and organize modules to compile as static or dynamic libraries (.so/.a, .lib/.dll) and link into the final application | 2 | Aneesh |
| 4 | Add CMake flags to allow compiling only certain modules or configurations of the application | 2 | Aneesh |

#### Completed: ####
We created a modular build system using CMake that allows us to easily build and run the application without concern for operating system. The script includes configurations and checks for the necessary dependencies such as Qt, GoogleTest, and PortAudio. The process generates a library that can be linked to by third party developers and is used to shorten compile time during our builds. The user interface stage of the build is optional and can be skipped.

### **User Story #6: ** ###
As a developer, I would like to setup a continuous integration (CI) process so that each repository push triggers a build and runs tests.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create Jenkins pipelines for Windows, Linux, and macOS | 3 | Jack |
| 2 | Configure dependencies on each server | 2 | Jack |
| 3 | Create build and test scripts in the scripting language of each OS | 3 | Aneesh |
| 4 | Generate code coverage reports after test suite completion and upload to codecov.io | 1 | Aneesh |

### Completed: ###
We setup a consistent and reliable continuous integration pipeline using Jenkins CI on three distributed servers (https://jenkins.symboxtra.dynu.net/job/HulaLoop). The pipeline builds and tests the application on each of the targeted operating systems. The environment configured on each of the systems contains all of our required dependencies and is cleanly reset each build to ensure stability. Code coverage from each of the builds is uploaded and tracked.

### **User Story #7:** ###
As a user, I would like to have a GUI.


| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Implement bridge between C++ and QML | 2 | Aneesh |
| 2 | Layout the GUI components using a layout manager | 2 | Aneesh |

#### Completed: ####
A bridge between QML and C++ was created allowing for easy C++ method calls from the UI. The GUI was created using layouts allowing for scaling on multiple resolutions. Color schemes and styles were designed beforehand and implemented into the UI.

### **User Story #8:** ###
As a user, I would like to have a button to start audio recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled start button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |

#### Completed: ####
A stylized record button was added to the UI and integrated via our C++/QML bridge. We verify that it sends a signal to the C++ backend via a unit test that checks to see if the state of the program was changed to recording.

### **User Story #9:** ###
As a user, I would like to have a button to stop audio recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled stop button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |

#### Completed: ####
A stylized stop button was added to the UI and integrated via our C++/QML bridge. We verify that it sends a signal to the C++ backend via a unit test that checks to see if the state of the program was changed to stop recording.

### **User Story #10:** ###
As a user, I would like to have a button that starts playback of recorded audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled play button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |

#### Completed: ####
A stylized start button was added to the UI and integrated via our C++/QML bridge. We verify that it sends a signal to the C++ backend via a unit test that checks to see if the state of the program was changed to play audio.

### **User Story #11:** ###
As a user, I would like to have a button that pauses playback of recorded audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled pause button to layout manager | 1 | Aneesh |
| 2 | Integrate button with Transport backend | 1 | Aneesh |
| 3 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |

#### Completed: ####
A stylized pause button was added to the UI and integrated via our C++/QML bridge. We verify that it sends a signal to the C++ backend via a unit test that checks to see if the state of the program was changed to pause audio.

### **User Story #12:** ###
As a user, I would like to delay the start of a recording by a specified amount of time.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled timer button to layout manager | 1 | Emanuel |
| 2 | Add drop-down list with time options | 1 | Emanuel |
| 3 | Add textfield to show remaining countdown time | 1 | Emanuel |
| 4 | Integrate timer with Transport | 2 | Emanuel |
| 5 | Unit Test - Verify the button sends the expected signal | 0.25 | Emanuel |
| 6 | Unit Test - Various timer options successfully result in starting a recording |  0.25 | Emanuel |

#### Completed: ####
A stylized timer button was added to the UI and integrated via our C++/QML bridge. Instead of implementing a drop-down list, we created a text input field. The countdown timer displays the delay timer by counting down to 0. We opted out of integrating the timer with the transport because we were able to write the timer in QML without any need for background interaction. We verify via a unit test that when the countdown ends, a signal to start recording is sent. We also test that various inputs into the timer box work via a unit test.

### **User Story #13:** ###
As a user, I would like to record audio for a specified amount of time.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled timer button to layout manager | 1 | Aneesh |
| 2 | Add drop-down list with time options | 1 | Aneesh |
| 3 | Add textfield to show remaining recording time | 1 | Aneesh |
| 4 | Integrate timer with Transport | 2 | Aneesh |
| 5 | Unit Test - Verify the button sends the expected signal | 0.25 | Aneesh |
| 6 | Unit Test - Various timer options successfully result in stopping a recording | 0.25 | Aneesh |

#### Completed: ####
A stylized timer button was added to the UI and integrated via our C++/QML bridge. Instead of implementing a drop-down list, we created a text input field. The recording timer displays the recording time by counting up to the time specified. We opted out of integrating the timer with the transport because we were able to write the timer in QML without any need for background interaction. We verify via a unit test that when the recording time is up, a signal to stop recording is sent. We also test that various inputs into the timer box work via a unit test.

### **User Story #14:** ###
As a developer, I would like a documented API so that third-party developers can use this system in their applications.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Perform code reviews and establish coding standards | 4 (1 per person) | Aneesh, Emanuel, Jack, Neel |
| 2 | Setup code quality and static analysis tools | 1 | Emanuel |
| 3 | Act on results of code quality, static analysis, and code coverage to improve existing code | 12 (1 per person per week) | Aneesh, Emanuel, Jack, Neel |
| 4 | Automatically generate documentation from code using Doxygen | 3 | Jack |
| 5 | Host auto-updated and versioned documentation on readthedocs.io | 3 | Neel |

#### Completed: ####
We established coding standards at the beginning of the sprint. The rules have been enforced using an online static analysis tool. A quality grade is assigned to our code base and is reassessed with each new push. Each pull request into the stable branch of the repository requires a review (and understanding of the code written) from another team member. As a team, we acted on the results of this analysis to improve our code and fix issues pointed out by the autonomous tools or our own human reviews. Our general user-facing documentation is hosted at https://hulaloop.readthedocs.io/en/latest/ and is auto-generated and versioned with each push. Our developer documentation is a subset of the same site: https://hulaloop.readthedocs.io/en/latest/_static/doxygen/index.html and is built from code with Doxygen.

### **User Story #15:** ###
As a user, I would like to have a visualizer.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research and understand project/technology | 5 | Meriem (New Team Member) |
| 2 | Create separate QML window/layout | 2 | Meriem |
| 3 | Create visualizer mockups | 3 | Meriem |
| 4 | Implement mockup in QML | 4 | Meriem |
| 5 | Make visualizer responsive with mocked data | 3 | Meriem |

#### Completed: ####
A mockup visualizer was created inside along with the rest of the UI. It responds and shows visual feedback based off of random data.

## **What Did Not Go Well?** ##
At the end of the sprint, we were unable to completely cover all our testing scenarios. The initial tests run revealed more problems in our memory management which forced us to change our focus. Although we did not utilize CPack to generate cross platform installers, we researched the various generators that are required (NSIS, DMG, DEB, etc.) and have a plan to generate them for the next sprint.

### **User Story #3:** ###
As a developer, I would like to get an audio sample from Mac OS X.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 6 | Unit Test - Read from input and output devices to ensure sampling process works correctly | 1 | Jack |

#### Not Completed: ####
As noted above, macOS presented a challenge for us during this sprint. We discovered that JACK’s JackRouter audio driver was broken and were forced to work out our own driver in its place. This was not something we anticipated, but we were luckily able to complete the driver integration and other macOS tasks during this sprint. The generic all-OS unit tests that we wrote cover some of macOS audio process, but deeper unit testing escaped us as the driver came to fruition in the later stages of the sprint.

### **User Story #5:** ###
As a developer, I would like the build system to be modular and support cross-platform compilation.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 5 | Experiment with CPack to create a basic installer | 3 | Aneesh |

#### Not Completed: ####
We decided not to create the installers during this sprint because we felt that we did not have a fully cohesive version of the application.

### **User Story #7:** ###
As a user, I would like to have a GUI.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 3 | Design color scheme and styles | 1 | Aneesh |
| 4 | Implement color scheme and styles | 1 | Aneesh |

#### Not Completed: ####
Although we created a GUI that has a rough color scheme and style (i.e. Material design, dark theme with blue and orange highlights), the styling that has been applied is not the finished product. We plan to demo the application to some users and hope to modify the color scheme and style based on feedback.

## **How Should We Improve?** ##
This sprint was an excellent learning experience for our entire team. As it was the first sprint, we began by overcoming a learning curve familiarizing ourselves with the OS level interfaces. We did run into some issues throughout the sprint that included time management and some programming specific issues.


In terms of time management, we not only underestimated how long things would take, but also assigned ourselves a full three-weeks worth of hours for a two-week sprint. These two factors combined, pushed us hard as a team. The addition of a new team member half-way through the sprint also required us to divert time away from other tasks. Overall, we finished everything in the sprint well, but completely overworked ourselves. This is something we aim to avoid in the next sprint. Since more work occured in the tail-end of the sprint than the beginning (see Burndown report below), we also plan to better distribute our workload throughout the sprint.


Programming specific issues arose during development. We quickly realized that our memory management strategy was rather naive, but were unable to perfect it during this sprint. As a result, we leaked a few bytes and enjoyed some SIGSEGVs before reaching a working prototype. In the future, we plan to be more thoughtful and controlling of our memory usage as well as our other resources. As always, we hope to write cleaner and more manageable code.

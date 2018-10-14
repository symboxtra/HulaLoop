![Hulaloop](/res/logo/hulaloop-header-small.png)

# **Sprint 2 Planning Document** #
Team 30: Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Meriem Bounab, Neel Patel

## **Sprint Overview** ##
During this sprint, we plan to solidify and build off the foundation of our application. We plan to implement more of our essential features. These features include the actual recording process, allowing the user to save the audio to a file, notifying the user of recording, and adding our command line interface. We continue to focus on our API for third-party developers, including documentation of memory management techniques for them and ourselves. We also plan to excel in code quality by keeping our code coverage and code factor scores high and continuing good documentation practices.

## **Scrum Master:** ##

Neel Patel


## **Meeting Plan:** ##
Monday, Wednesday, Friday 5:00 PM - 7:00 PM

## **Risks and Challenges** ##
Since this is the second sprint, we are far better prepared for the risks and challenges that might come our way. The primary challenges that we saw in the first sprint have mostly been conquered. The only lingering challenge is the unfamiliar APIs and technologies. We still have a few to introduce and remain new to the ones that we have been using. Memory management also pained us last sprint. To rectify this, we plan to develop a memory management strategy. This looks like it will be rather challenging as we allocate buffers that must be used in multiple parts of the application and at different times. Part of this challenge is also incorporating safe ways for third-party developers to receive and then deallocate memory from our library.

## **Current Sprint Detail** ##
### **User Story #16:** ###
As a user, I would like to choose an input device to record audio from.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Emanuel |
| 2 | Make sure the device has the proper capabilities | 1 | Emanuel |
| 3 | Ensure valid sampling and bit rates are available | 2 | Emanuel |

#### Acceptance Criteria: ####
* Given that the input device exists, when the user selects it, the backend receives the information about the switch.
* Given that the input device does not exists, the backend will notify the user that there is an error and display a message that the input device does not exist.
* Given that the input device has an invalid sampling or bit rate, the user will be notified that the sampling and bit rate they choose does not exist on the device.

### **User Story #17:** ###
As a user, I would like to choose an output device to record audio from (loopback).

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Emanuel |
| 2 | Make sure that the device has proper capabilities | 1 | Emanuel |
| 3 | Ensure valid sampling and bit rates are available | 2 | Emanuel |

#### Acceptance Criteria: ####
* Given that the output device exists, when the user selects it, the backend receives the information about the switch.
* Given that the output device does not exists, the backend will notify the user that there is an error and display a message that the output device does not exist.
* Given that the output device has an invalid sampling or bit rate, the user will be notified that the sampling and bit rate they choose does not exist on the device.

### **User Story #18:** ###
As a user, I would like to record audio for a specified amount of time.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a bridge between the Transport and the Record class | 3 | Emanuel |
| 2 | Register Record class with the Controller to receive audio data | 2 | Emanuel |
| 3 | Unregister with the Controller class when the timer reaches the desired duration | 2 | Emanuel |
| 4 | Unit Test - Verify that state between the Transport and Record class is synced | 2 | Emanuel |
| 5 | Unit Test - Verify the registration and deregistration of the Record class with the Controller class | 2 | Emanuel |

#### Acceptance Criteria: ####
* Given that the recording timer is precise, the recording should stop when the counter reaches the desired record duration and be within 0.5s of the desired time.
* Given that the timer countdown sends the correct recording signal, the recording should start after the countdown.
* Given that there is no input (or infinity) for the record duration, the application should record indefinitely until the user stops it.

### **User Story #19:** ###
As a user, I would like to record until I manually stop the recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Remove self from callback list when stop button is triggered | 1 | Neel |
| 2 | Design and implement UI transport control state machines for stop button | 3 | Neel |
| 3 | Update other UI buttons based on the state machine | 1 | Neel |
| 4 | Unit Test - Verify that the callback is removed when the stop button is pressed | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks and states | 2 | Neel |

#### Acceptance Criteria: ####
* Given that the callback is removed when the stop button is pressed, the OS layer should stop capturing audio data from the system.
* Given that the stop button is pressed, the record and stop buttons should be disabled and the captured data should be ready to export or discard.
* Given that the stop button is pressed, the play button will be enabled.
* Given that the stop button is pressed, the export button will be activated and the audio data can be exported to a file.

### **User Story #20:** ###
As a user, I would like to pause the capture process.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Remove self from callback list when pause button is triggered | 1 | Neel |
| 2 | Design and implement UI transport control state machines for pause button | 3 | Neel |
| 3 | Update other UI buttons based on the state machine | 1 | Neel |
| 4 | Unit Test - Verify that the callback is removed when the pause button is pressed | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks | 2 | Neel |

#### Acceptance Criteria: ####
* Given that the callback is removed when the pause button is pressed, the OS layer should stop capturing audio data from the system.
* Given that the record button is pressed, the playback and record button are disabled.
* Given that the record button is pressed, the visualizer will start moving based on the audio provided.

### **User Story #21:** ###
As a user, I would like to resume the capture process.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add self to the OS callback again when record button is clicked again | 1 | Neel |
| 2 | Design and implement UI transport control state machines for record button | 3 | Neel |
| 3 | Design and implement UI transport control state machines for record button | 1 | Neel |
| 4 | Unit Test - Verify that the callback is added back to the callback list | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks and states | 2 | Neel |

#### Acceptance Criteria: ####
* Given that the callback is removed when the record button is pressed, the OS layer should starts capturing audio data again from the system.
* Given that the pause button is pressed, all UI buttons except the playback, stop and record buttons should be disabled.
* Given that the pause button is manually pressed, the visualizer will stop moving since no audio is being provided.

### **User Story #22:** ###
As a user, I would like to visualize audio from the selected device.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Adapt visualizer width to auto-fill to variant window width | 2 | Meriem |
| 2 | Read raw audio data from buffer | 2 | Meriem |
| 3 | Create algorithm to convert raw audio data to height | 4 | Meriem |
| 4 | Create algorithm to separate data into variant amounts of bins | 5 | Meriem |
| 5 | Make visualizer pause and maintain prior heights when pause button is pressed | 3 | Meriem |
| 6 | Make visualizer resume when play button is pressed | 2 | Meriem |
| 7 | Create a dynamic line that is able to follow the variant heights of the visualizer | 6 | Meriem |

#### Acceptance Criteria: ####
* Given  that the visualizer can read raw audio data, the visualizer is able to dynamically move according to the read data.
* Given that the user can change the window size, visualizer is able to adapt to different window sizes by adding or removing bins accordingly.
* Given that the user pressed the pause and play buttons, the visualizer stops and resumes movement.
* Given that the original visualizer has dynamic heights, the visualizer has a dynamic curved line that is able to follow the same height movements.

### **User Story #23:** ###
As a user, I would like to export captured audio to a file.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a button that allows the user to export the audio file | 1 | Meriem |
| 2 | Create a dialog box that shows up when the export button is pressed | 2 | Meriem |
| 3 | Create a cross-platform SaveFileDialog that allows for renaming files and saving to a specific directory | 5 | Emanuel |
| 4 | Check for errors when saving the file | 1 | Emanuel |
| 5 | Unit test: Verify error handling and correct file saving behavior | 3 | Meriem |

#### Acceptance Criteria: ####
* Given that the audio was written to a temporary file, the UI should have an option to export the final file to a local directory.
* Given that the export button is clicked, a save file dialog box should show up allowing the user to export the recorded audio.
* Given that the file is savable to a local directory, the SaveFileDialog should allow you name the file.

### **User Story #24:** ###
As a user, I would like to export captured audio in WAV format.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Integrate WAV encoding libraries per platform | 4 | Neel & Jack |
| 2 | Acquire all raw temporary audio files and merge into single audio buffer | 2 | Neel |
| 3 | Encode the PCM audio buffer into WAV format | 3 | Neel |
| 4 | Use the cross-platform SaveFileDialog to store the WAV file to a specific local directory | 3 | Neel |
| 5 | Unit Test - Verify WAV encoding integrity | 2 | Neel |

#### Acceptance Criteria: ####
* Given that the WAV encoding library is integrated into the application, the application should be able to utilize this library to encode PCM audio.
* Given that the PCM audio data is encoded into WAV format, the SaveFileDialog can store the WAV file to a specific local directory.
* Given that the WAV file is stored to a local directory, the WAV file should not be corrupted and be able to play the audio when the file is run.

### **User Story #25:** ###
As a third-party developer, I would like to set the device from which I capture audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Jack |
| 2 | Initiate new callback threads once Controller receives device switch from third-party application | 1 | Jack |
| 3 | Expose a subset of our Controller API to third-parties for switching devices | 1 |	Jack |
| 4 | Create documentation detailing how third-party developers should access device lists and use the exposed API | 4 | Meriem |

#### Acceptance Criteria: ####
* Given that the devices exist, when the user selects a device, the backend receives the information about the switch.
* Given that a third-party developer can set the desired audio capture device, the third-party developer can receive audio data from the desired device.
* Given that the external documentation is completed, a third-party developer can easily determine how to access devices and switch the current device.

### **User Story #26:** ###
As a developer, I would like my application to have minimal memory leaks.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Determine memory management strategy. | 5 | Emanuel, Jack, Neel |
| 2 | Implement the memory management strategy. | 7 | Emanuel, Jack, Neel |
| 3  | Document the strategy for internal use and third-party developers. | 3 | Emanuel, Jack, Neel |
| 4 | Create valgrind tests to quantitatively measure memory leaks. | 4 | Emanuel, Jack, Neel |

#### Acceptance Criteria: ####
* Given that we minimize the memory leaks in our application, the amount of leaked memory should be 0 bytes.
* Given that the memory management strategy is documented, Doxygen should contain the updated developer documentation containing memory management tips and tools for other developers.
* Given that valgrind tests are created to monitor the application’s memory management, the valgrind tests should be integrated with our build process and fail the process if they are memory leaks present.

### **User Story #27:** ###
As a developer, I would like at least 70% of our code to be covered by the test suites.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Act on results of code quality, static analysis, and code coverage to improve existing code | 15 (1 per week) | Aneesh, Emanuel, Jack, Meriem, Neel |
| 2 | Add additional unit tests to cover more code | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |
| 3 | Remove dead code as indicated by missing coverage | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |

#### Acceptance Criteria: ####
* Given that code coverage is at an acceptable level, codecov.io will show 70% or greater coverage.
* Given that code coverage is at an acceptable level, the test suite will expand as we write additional code.
* Given that we review the results of our code coverage, dead code will be found and removed from the codebase.

### **User Story #28:** ###
As a user, I would like to be informed when the application is recording system audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research system tray icons | 1 | Aneesh |
| 2 | Create a cross-platform system tray icon | 3 | Aneesh |
| 3 | Update system tray icon based on recording state | 1 | Aneesh |
| 4 | Send a balloon message to user notifying recording | 2 | Aneesh |

#### Acceptance Criteria: ####
* Given that the application starts, a system tray icon will show up in the system tray of the operating system.
* Given the system tray icon exists, when the user starts recording, the icon changes to indicate recording.
* Given the system tray icon exists, when the user starts recording, a message balloon is displayed to the user.

### **User Story #29:** ###
As a user, I would like my application to stay up-to-date.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Start Updater process when launching the application | 1 | Aneesh |
| 2 | Create Updater GUI with progress bar and text labels | 3 | Aneesh |
| 3 | Query GitHub Releases API to determine if an update is available | 2 | Aneesh |
| 4 | If an update is found, download the release asset | 1 | Aneesh |
| 5 | Unpack downloaded ZIP containing necessary application files | 3 | Aneesh |
| 6 | Warn user about running instances of HulaLoop if update is ready to install | 1 | Aneesh |
| 7 | Remove all older instances of application files | 2 | Aneesh |
| 8 | Launch updated HulaLoop application | 1 | Aneesh |

#### Acceptance Criteria: ####
* Given that the user launches the application, the Updater process begins.
* Given that an update exists, when the update is downloaded, the progress bar in the Updater GUI shows the download status.
* Given that the update has been downloaded, the update is unpacked in the application directory.
* Given that the update is ready to install, the user is notified if there are any running instances of HulaLoop.
* Given that the update is ready to install, the older version of the application is removed and the updated version is launched.

### **User Story #30:** ###
As a user, I would like the installation process to be both intuitive and take less than three minutes.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Generate cross-platform installers using CPack | 3 | Aneesh |
| 2 | Customize install process | 7 | Aneesh |
| 3 | Create cross-platform uninstallers within the CPack generators | 3 | Aneesh |

#### Acceptance Criteria: ####
* Given that the cross-platform installers exist, a user is able to install the application on all supported operating systems.
* Given that the user runs the installer, the user is presented with intuitive and customized install options.
* Given that the user has installed the application on their computer, when the user runs the uninstaller, then the application and all pertinent files are removed from the computer.

### **User Story #31:** ###
As a user, I would like to be able to check for updates manually within the application.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled check update button to layout manager | 1 | Aneesh |
| 2 | Notify user if an update is available | 1 | Aneesh |
| 3 | Start update process | 1 | Aneesh |

#### Acceptance Criteria: ####
* Given that the check update button is styled, the button’s design matches the theme of the rest of the GUI.
* Given that the check update button exists, when the user clicks the button, then a message is displayed to the user indicating whether an update is available or not.
* Given that an update exists and the user confirms, the update process begins.

### **User Story #32:** ###
As a user, I would like the CLI to provide the same features as the GUI.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create an interactive CLI with a set of commands | 10 | Jack & Meriem |
| 2 | Create a non-interactive CLI using a set of flags | 5 | Jack & Meriem |
| 3 | Support transport controls and export | 7 | Jack |
| 4 | Add a man page/help dialog displaying all flags and commands | 5 | Jack |

#### Acceptance Criteria: ####
* Given that the interactive CLI is available, when the user starts the CLI, they are greeted with a prompt and can input commands.
* Given that the non-interactive CLI is available, the user is able to specify all required parameters and complete their desired operation.
* Given that the man page/help dialog is available, when the user is unsure how to perform an operation, the dialog presents the needed information.

## Remaining Product Backlog ##

### Functional Requirements ###
1. ~~As a user, I would like to view all available audio devices.~~
2. ~~As a user, I would like to choose an input device to record audio from.~~
3. ~~As a user, I would like to choose an output device to record audio from (loopback).~~
4. As a user, I would like to choose the output device on which I hear monitored audio.
5. ~~As a user, I would like to have a GUI.~~
6. ~~As a user, I would like to have a button to start audio recording.~~
7. ~~As a user, I would like to have a button to stop audio recording.~~
8. ~~As a user, I would like to delay the start of a recording by a specified amount of time.~~
9. ~~As a user, I would like to record audio for a specified amount of time.~~
10. ~~As a user, I would like to record until I manually stop the recording.~~
11. As a user, I would like the application to automatically write to a new file when file size limit is exceeded so that recording can continue seamlessly.
12. ~~As a user, I would like to pause the capture process.~~
13. ~~As a user, I would like to resume the capture process.~~
14. As a user, I would like to  adjust the volume of monitored audio (if time allows).
15. ~~As a user, I would like to visualize audio from the selected device (if time allows).~~
16. As a user, I would like to trim captured audio (if time allows).
17. As a user, I would like to add audio filters (if time allows).
18. As a user, I would like to play back what was most recently recorded.
19. ~~As a user, I would like to have a button that starts playback of recorded audio.~~
20. ~~As a user, I would like to have a button that pauses playback of recorded audio.~~
21. ~~As a user, I would like to export captured audio to a file.~~
22. As a user, I would like to choose the bitrate for exported files.
23. As a user, I would like to choose the bit-depth for exported files.
24. ~~As a user, I would like to export captured audio in WAV format.~~
25. As a user, I would like to export captured audio in MP3 format.
26. As a user, I would like to export my audio in other formats (if time allows).
27. As a user, I would like to be able to link my Google Drive account (if time allows).
28. As a user, I would like to be able to link my Dropbox account (if time allows).
29. As a user, I would like to be able to upload captured audio to linked storage services (if time allows).
30. ~~As a developer, I would like to get an audio sample from Windows.~~
31. ~~As a developer, I would like to get an audio sample from Mac OS X.~~
32. ~~As a developer, I would like to get an audio sample from Linux.~~
33. ~~As a third party developer, I would like to set the device from which I capture audio.~~
34. ~~As a developer, I would like the program to have minimal memory leaks.~~

### Non-Functional Requirements ###

1. ~~As a developer, I would like the build system to be modular.~~
2. ~~As a developer, I would like the build system to support cross-platform compilation.~~
3. ~~As a developer, I would like to setup a continuous integration (CI) process so that each repository push triggers a build and runs tests.~~
4. ~~As a developer, I would like a documented API so that third-party developers can use this system in their applications.~~
5. As a developer, I would like the application to work on Windows, Mac OS X, and Debian-based Linux.
6. ~~As a third-party developer, I would like to easily use the project’s libraries in my application.~~
7. As a developer, I would like the application to have a long MTBF.
8. As a developer, I would like the system audio being recorded to be compressed on-the-fly to conserve disk space and extend maximum recording duration.
9. ~~As a developer, I would like at least 70% of our code to be covered by the test suites.~~
10. ~~As a user, I would like to be informed when the application is recording system audio.~~
11. ~~As a user, I would like my application to stay up-to-date.~~
12. ~~As a user, I would like the installation process to be both intuitive and take less than three minutes.~~
13. ~~As a user, I would like to be able to check for updates manually within the application.~~
14. As a user, I would like the GUI to be intuitive and straightforward.
15. ~~As a user, I would like the CLI to provide the same feature set as the GUI.~~

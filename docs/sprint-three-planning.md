![Hulaloop](/res/logo/hulaloop-header-small.png)

# **Sprint 3 Planning Document** #
Team 30: Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Meriem Bounab, Neel Patel

## **Sprint Overview** ##
During this sprint, we plan to complete the application for our first release. Some of the final essential features that we plan to implement are the playback of recorded audio, additional codecs, and error handling. Our third-party API is complete, including documentation, but we also plan to add some example programs this sprint. As always, we plan to excel in code quality by keeping our code coverage and code factor scores high and continuing good documentation practices.

## **Scrum Master:** ##
Aneesh Gokhale


## **Meeting Plan:** ##
Monday, Wednesday, Friday 5:00 PM - 7:00 PM

## **Risks and Challenges** ##
Since this is the third and final sprint, we are far better prepared for the risks and challenges that might come our way. Being the last sprint, there is a great chance that we will find ourselves spending a lot of time tying up loose ends and cleaning up existing features. With this in mind, we have planned a relatively feature-light sprint to ensure that we have time to make the existing features as robust as possible.

## **Current Sprint Detail** ##
### **User Story #33:** ###
As a user, I would like to play back what was most recently recorded.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a UI to represent the recorded audio | 10 | Meriem |
| 2 | Transition to the new UI from the visualizer | 3 | Meriem |
| 3 | Use PortAudio on Windows to playback audio | 7 | Neel |
| 4 | Use PortAudio on Linux to playback audio | 7 | Emanuel |
| 5 | Use PortAudio on OSX to playback audio | 7 | Jack |
| 6 | Determine the file to play based on the Transport position | 5 | Neel |

#### Acceptance Criteria: ####
- Given that playback is implemented on Windows, the user should be able to listen to recorded audio on the selected output device
- Given that playback is implemented on Linux, the user should be able to listen to recorded audio on the selected output device
- Given that playback is implemented on OSX, the user should be able to listen to recorded audio on the selected output device

### **User Story #34:** ###
As a user, I would like to export captured audio in MP3 format.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Find a cross-platform MP3 encoding library | 3 | Jack |
| 2 | Integrate the MP3 encoding library | 4 | Jack |
| 3 | Add the MP3 export option to the GUI and CLI | 2 | Jack |

#### Acceptance Criteria: ####
- Given that a cross-platform MP3 library has been found, the library is available within the application.
- Given that the MP3 library is integrated, files can be exported in MP3 format.
- Given that the MP3 option is added to the CLI and GUI, the user can choose the export format.

### **User Story #35:** ###
As a user, I would like to export my audio in other formats (if time allows).

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Use libsndfile to export in other audio formats such as WAV, FLAC, CAF, AIFF | 3 | Neel |
| 2 | Allow the user to choose the export format | 3 | Neel |

#### Acceptance Criteria: ####
- Given that multiple audio formats are supported, the export module contains configurations for each of the codecs.
- Given that the application can encode in the selected format, audio is saved in the specified format and is playable.
- Given that multiple audio formats are supported, the user can choose one of them as the export format.

### **User Story #36:** ###
As a developer, I would like the system audio being recorded to be compressed on-the-fly to conserve disk space and extend maximum recording duration.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Use FLAC encoding to store audio data in system temp folder | 4 | Jack & Neel |
| 2 | After a recording is exported, clean up temp files from the previous recording | 2 | Neel |

#### Acceptance Criteria: ####
- Given that the audio is compressed on-the-fly, the compressed temporary file is smaller than the uncompressed temporary file.
- Given that a recording was discarded, the temporary files are cleaned up.
- Given that the program was closed properly, the temporary files are cleaned up.

### **User Story #37:** ###
As a developer, I would like the application to have a long MTBF.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create exceptions and implement exception handlers | 6 | Aneesh, Emanuel, Jack, Meriem, Neel |
| 2 | Display helpful messages to the user when an exception is caught and recover if possible | 3 | Aneesh & Emanuel |
| 3 | Test - Create stress tests to find points of failure in the application | 4 | Aneesh & Jack |
| 4 | Clean up include guards and create HulaLoop namespace to prevent issues when used by third-party developers | 3 | Aneesh & Jack |

#### Acceptance Criteria: ####
- Given that the custom exceptions exist, they are thrown when appropriate errors are encountered.
- Given that the exception is caught, the exception handler attempts to recover from the error if possible.
- Given that the exception is caught, the user is presented with a descriptive message about the error.

### **User Story #38:** ###
As a user, I would like to choose the output device on which I hear monitored audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify backend of the output device switch | 2 | Emanuel |
| 2 | Join and kill playback threads and restart new threads | 2 | Neel & Jack |
| 3 | Default output device to the first device in output device list | 2 | Emanuel |

#### Acceptance Criteria: ####
- Given that the output device is selected, the backend receives proper the signal, and it is displayed on the UI.
- Given that there is no active output device selected the default audio device is selected.
- Given that there are output devices at least one of them is shown on the UI at all times.

### **User Story #39:** ###
As a user, I would like to visualize audio from the selected device

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Translate points for line visualizer into a circular formation | 4 | Meriem |
| 2 | Have line that forms a circle pulse outwards from the center at different angles | 4 | Meriem |
| 3 | Transition the GUI to the recorded display from the visualizer | 4 | Meriem |
| 4 | Allow switching of visualization mode based on setting | 3 | Meriem |

#### Acceptance Criteria: ####
- Given that this user can change the visualization mode, the visualizer will switch modes accordingly.
- Given that the points along the line are translated to form a circle, the line visualizer can pulse outwards.
- Given that the user presses the button to display the timeline editor, the visualizer can transition to that display.

### **User Story #40:** ###
As a user, I would like to set and save settings.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled settings button to layout manager | 1 | Aneesh |
| 2 | Create a settings window and add buttons and labels | 5 | Meriem |
| 3 | Allow changing settings from the CLI | 2 | Jack |
| 4 | Allow changing settings from the GUI | 2 | Aneesh |
| 5 | Save settings to disk when changed by the user | 3 | Aneesh |
| 6 | Separate CLI and GUI settings into their own modules | 2 | Aneesh |
| 7 | Load settings on startup | 5 | Aneesh |

#### Acceptance Criteria: ####
- Given that the styled settings button exists, when the user clicks on it, a settings menu pops up.
- Given that the settings module exists, CLI and GUI users can modify settings.
- Given that the settings file exists, the application parses it and applies the settings on startup.

### **User Story #41:** ###
As a user, I would like the GUI to be intuitive and straightforward.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Redesign any interfaces that are too deeply nested | 3 | Meriem |
| 2 | Add tooltips to GUI components | 2 | Meriem |
| 3 | Create Qt translation files | 3 | Jack |
| 4 | Translate to known languages (Polish, Spanish, French, etc) | 10 | Aneesh, Emanuel, Jack, Meriem, Neel |
| 5 | Incorporate translations into GUI | 3 | Jack |

#### Acceptance Criteria: ####
- Given that the GUI is intuitive, the number of clicks to reach essential features should be less than 3.
- Given that the GUI is intuitive to global users, multiple languages are supported (with the ability to add more).
- Given that multiple languages are supported, the GUI shows the correct translation.

### **User Story #42:** ###
As a user, I would like to have a warning dialog on exit when there is unsaved audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Override exit function in Qt | 2 | Emanuel |
| 2 | Create an exit dialog that pops up if the user has unsaved files | 4 | Emanuel |
| 3 | Delete temporary files on close of the application | 2 | Emanuel |

#### Acceptance Criteria: ####
- Given that the user has unsaved files a dialog will pop up asking the user if they want to exit.
- Given that the user selects no, the dialog will close and go back to the application.
- Given that the user selects yes, the dialog will close along with the application and the temporary files are removed.

#### User Story #43: ####
As a third-party developer, I would like example programs demonstrating how to use the library.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a very basic example | 3 | Neel & Jack |
| 2 | Create a more complex example | 4 | Neel & Jack |
| 3 | Create an advanced example | 6 | Neel & Jack |

#### Acceptance Criteria: ####
- Given that a basic example is created, a novice user could use it as a guide for their own development.
- Given that a more complex example is created, an intermediate user could use it as a guide for their own development.
- Given that an advanced example is created, an advanced user could use it as a guide for their own development.

### **User Story #44:** ###
As a developer, I would like at least 70% of our code to be covered by the test suites.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Act on results of code quality, static analysis, and code coverage to improve existing code | 15 (1 per week) | Aneesh, Emanuel, Jack, Meriem, Neel |
| 2 | Add additional unit tests to cover more code | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |
| 3 | Remove dead code as indicated by missing coverage | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |

#### Acceptance Criteria: ####
- Given that code coverage is at an acceptable level, codecov.io will show 70% or greater coverage.
- Given that code coverage is at an acceptable level, the test suite will expand as we write additional code.
- Given that we review the results of our code coverage, dead code will be found and removed from the codebase.

## Remaining Product Backlog ##
### Functional Requirements ###
1. ~~As a user, I would like to view all available audio devices.~~
2. ~~As a user, I would like to choose an input device to record audio from.~~
3. ~~As a user, I would like to choose an output device to record audio from (loopback).~~
4. ~~As a user, I would like to choose the output device on which I hear monitored audio.~~
5. ~~As a user, I would like to have a GUI.~~
6. ~~As a user, I would like to have a button to start audio recording.~~
7. ~~As a user, I would like to have a button to stop audio recording.~~
8. ~~As a user, I would like to delay the start of a recording by a specified amount of time.~~
9. ~~As a user, I would like to record audio for a specified amount of time.~~
10. ~~As a user, I would like to record until I manually stop the recording.~~
11. ~~As a user, I would like the application to automatically write to a new file when file size limit is exceeded so that recording can continue seamlessly.~~
12. ~~As a user, I would like to pause the capture process.~~
13. ~~As a user, I would like to resume the capture process.~~
14. As a user, I would like to adjust the volume of monitored audio (if time allows).
15. ~~As a user, I would like to visualize audio from the selected device.~~
16. As a user, I would like to trim captured audio (if time allows).
17. As a user, I would like to add audio filters (if time allows).
18. ~~As a user, I would like to play back what was most recently recorded.~~
19. ~~As a user, I would like to have a button that starts playback of recorded audio.~~
20. ~~As a user, I would like to have a button that pauses playback of recorded audio.~~
21. ~~As a user, I would like to export captured audio to a file.~~
22. ~~As a user, I would like to choose the bitrate for exported files.~~
23. ~~As a user, I would like to choose the bit-depth for exported files.~~
24. ~~As a user, I would like to export captured audio in WAV format.~~
25. ~~As a user, I would like to export captured audio in MP3 format.~~
26. ~~As a user, I would like to export my audio in other formats (if time allows).~~
27. As a user, I would like to be able to link my Google Drive account (if time allows).
28. As a user, I would like to be able to link my Dropbox account (if time allows).
29. As a user, I would like to be able to upload captured audio to linked storage services (if time allows).
30. ~~As a developer, I would like to get an audio sample from Windows.~~
31. ~~As a developer, I would like to get an audio sample from Mac OS X.~~
32. ~~As a developer, I would like to get an audio sample from Linux.~~
33. ~~As a third party developer, I would like to set the device from which I capture audio.~~
34. ~~As a developer, I would like the program to have minimal memory leaks.~~
35. ~~As a third-party developer, I would like example programs demonstrating how to use the library.~~
36. ~~As a user, I would like to have a warning dialog on exit when there is unsaved audio files.~~
37. ~~As a user, I would like to set and save personal settings.~~

### Non-Functional Requirements ###
1. ~~As a developer, I would like the build system to be modular.~~
2. ~~As a developer, I would like the build system to support cross-platform compilation.~~
3. ~~As a developer, I would like to setup a continuous integration (CI) process so that each repository push triggers a build and runs tests.~~
4. ~~As a developer, I would like a documented API so that third-party developers can use this system in their applications.~~
5. ~~As a developer, I would like the application to work on Windows, Mac OS X, and Debian-based Linux.~~
6. ~~As a third-party developer, I would like to easily use the project’s libraries in my application.~~
7. ~~As a developer, I would like the application to have a long MTBF.~~
8. ~~As a developer, I would like the system audio being recorded to be compressed on-the-fly to conserve disk space and extend maximum recording duration.~~
9. ~~As a developer, I would like at least 70% of our code to be covered by the test suites.~~
10. ~~As a user, I would like to be informed when the application is recording system audio.~~
11. ~~As a user, I would like my application to stay up-to-date.~~
12. ~~As a user, I would like the installation process to be both intuitive and take less than three minutes.~~
13. ~~As a user, I would like to be able to check for updates manually within the application.~~
14. ~~As a user, I would like the GUI to be intuitive and straightforward.~~
15. ~~As a user, I would like the CLI to provide the same feature set as the GUI.~~
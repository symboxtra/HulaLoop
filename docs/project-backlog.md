![HulaLoop Logo](res/logo/hulaloop-header-small.png)

# Team 30 Project Backlog #

### **Team Members:** ###
Aneesh Gokhale, Emanuel Pituch, John McKernan, Neel Patel


### **Problem Statement:** ###
There are many technologies for capturing and routing system-wide audio. These technologies vary widely by operating system and can be difficult for developers to interact with and for users to understand. While there are existing technologies that provide a loopback interface for capturing this audio, their features are limited, and none are completely cross-platform. By delivering a library for developers and an easy-to-use UI for average consumers, anyone should be able to visualize, capture, and use their system audio as they see fit.


## **Background Information:** ##
### **Audience:** ###
Our audience includes any computer user who wants to record their system-wide audio or any developer who wants to create software that works with that same audio. Our application should cater to the full spectrum of these users: fully-featured enough to meet the needs of experts but still simple enough for novices.


### **Similar Platforms:** ###
Several existing options such as jack_capture, Windows Stereo Mix, Virtual Audio Cable, and Soundflower are capable of accessing system-wide audio. These applications serve various purposes: jack_capture can be used on a Linux or Unix based operating systems, Windows Stereo Mix and Virtual Audio Cable are built for Windows, and Soundflower can be used on Mac. All but jack_capture focus on the routing of audio rather than capturing it.


### **Limitations:** ###
The main limitation with these applications is their inability to provide a consistent and simple cross-platform experience. Many of them work well in their respective environments; we hold, however, that a user should have the freedom to move between environments at will without the setback of learning a new tool. Moreover, some of these tools only provide a command line interface. For users unfamiliar with the command line, this can be a severe limitation. Lastly, all but jack_capture require additional software to record and export audio.


We hope to solve this by crafting a simple, consistent GUI and CLI that are capable of accessing, recording, and exporting system-wide audio on a variety of operating systems.

## **Requirements** ##
### **Functional Requirements:** ###
1. As a user, I would like to view all available audio devices.
2. As a user, I would like to choose an input device to record audio from.
3. As a user, I would like to choose an output device to record audio from (loopback).
4. As a user, I would like to monitor the audio that is currently being captured.
5. As a user, I would like to choose the output device on which I hear monitored audio.
6. As a user, I would like to  adjust the volume of monitored audio (if time allows).
7. As a user, I would like to mute the audio monitoring.
8. As a user, I would like to visualize audio from the selected device (if time allows).
9. As a user, I would like to have a button to start audio recording.
10. As a user, I would like to have a button to stop audio recording.
11. As a user, I would like to delay the start of a recording by a specified amount of time.
12. As a user, I would like to record audio for a specified amount of time.
13. As a user, I would like to record until I manually stop the recording.
14. As a user, I would like the application to automatically write to a new file when file size limit is exceeded so that recording can continue seamlessly.
15. As a user, I would like to pause the capture process.
16. As a user, I would like to resume the capture process.
17. As a user, I would like to play back what was most recently recorded.
18. As a user, I would like to have a button that starts playback of recorded audio.
19. As a user, I would like to have a button that pauses playback of recorded audio.
20. As a user, I would like the application to be capable of minimizing to the system tray.
21. As a user, I would like to trim captured audio (if time allows).
22. As a user, I would like to export captured audio to a file.
23. As a user, I would like to choose the bitrate for exported files.
24. As a user, I would like to choose the bit-depth for exported files.
25. As a user, I would like to export captured audio in WAV format.
26. As a user, I would like to export captured audio in MP3 format.
27. As a user, I would like to export my audio in other formats (if time allows).
28. As a user, I would like to be able to link my Google Drive account (if time allows).
29. As a user, I would like to be able to link my Dropbox account (if time allows).
30. As a user, I would like to be able to upload captured audio to linked storage services (if time allows).
31. As a user, I would like to have a button that checks for updates to the application.
32. As a user, I would like to be notified when an update is available for the application.
33. As a user, I would like the application to automatically update (if time allows).
34. As a developer, I would like to download patches from within the application.
35. As a developer, I would like to install patches from within the application.
36. As a developer, I would like to setup a continuous integration (CI) process so that each repository push triggers a build and runs tests.
37. As a developer, I would like the test suite to run as part of the CI process.
38. As a developer, I would like a documented API so that third-party developers can use this system in their applications.
39. As a third-party developer, I would like to set the device from which I wish to capture.
40. As a third-party developer, I would like to pull captured audio data into my application in real time.


### **Non-Functional Requirements:** ###

#### Architecture: ####
Since we plan to support a variety of operating systems and configurations, it is necessary that we create a modular design and build system using CMake. We plan to make every effort possible to avoid duplication of work by designing with this in mind. This modular thinking will also simplify the process of assigning and completing work on the team. To enable quick prototyping, modern design, and cross-platform support, our frontend will be written in QML using the QtQuick framework. Our backend will be written in C++ and use the JACK (Linux and OSX) and WASAPI (Windows) APIs to communicate with lower level audio endpoints.


#### Performance: ####
Since the capture program is intended to run in the background while a user performs other tasks, it is imperative that resource usage from our application be as minimal as possible. Compression to the user’s desired bitrate (e.g. 128 kbps) should be applied on-the-fly in order to conserve disk space and extend the maximum possible record duration. Should a recording exceed the maximum file size of 4GB, an additional file should be created and written to, allowing recording to continue.


#### Security: ####
Since system-wide audio could include sensitive information such as phone calls or private conversations, we must ensure that the user is made aware anytime audio is actively being captured. Though this is not required by desktop operating systems, we aim to present this warning to enforce the good-natured use of our application and ensure the privacy of our users.


#### Usability: ####
We plan to support both a GUI and CLI form of the application. The GUI should be simple and straightforward for all users to navigate and use. The CLI should provide the same set of features and feel comfortable for more advanced users. The install process for both should be intuitive and take less than 3 minutes.

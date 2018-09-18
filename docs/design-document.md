![HulaLoop Logo](/res/logo/hulaloop-header-small.png)

# Design Document #
### Team 30 : Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Neel Patel ###

# Table of Contents #

* ## **[Purpose](#purpose)** ##
    * ### [Functional Requirements](#functional-requirements) ###
    * ### [Non-Functional Requirements](#non-functional-requirements) ###
* ## **[Design Outline](#design-outline)** ##
    * ### [High-Level Overview](#high-level-overview) ###
    * ### [Components](#components) ###
* ## **[Design Issues](#design-issues)** ##
    * ### [Functional Issues](#functional-issues) ###
    * ### [Non-Functional Issues](#non-functional-issues) ###
* ## **[Design Details](#design-details)** ##
    * ### [Class Design](#class-design) ###
    * ### [Class Description](#class-description) ###
    * ### [Sequence Of Events](#sequence-of-events) ###
    * ### [Activity Diagram](#activity-diagram) ###
    * ### [State Diagram](#state-diagram) ###
    * ### [UI  Mockups](#ui-mockups) ###


## **Purpose** ##
There are many technologies for capturing and routing system-wide audio.
These technologies vary widely by operating system and can be difficult for developers to interact with and for users to understand.
While there are existing technologies that provide a loopback interface for capturing this audio, their features are limited, and none are completely cross-platform.
Many of them work well in their respective environments; we hold, however, that a user should have the freedom to move between environments at will without the setback of learning a new tool. 
Moreover, some of these tools only provide a command line interface. For users unfamiliar with the command line, this can be a severe limitation. 


Our audience includes any computer user who wants to record their system-wide audio or any developer who wants to create software that works with that same audio. 
Our application should cater to the full spectrum of these users: fully-featured enough to meet the needs of experts but still simple enough for novices. 
By delivering a library for developers and an easy-to-use UI for average consumers, we hope to create the best of both worlds.

### **Functional Requirements** ###

#### Devices ####

As a user,

1. I would like to view all available audio devices.
2. I would like to choose an input device to record audio from.
3. I would like to choose an output device to record audio from (loopback).
4. I would like to choose the output device on which I hear monitored audio.

As a third-party developer,

1. I would like to set the device from which I capture audio.

#### Recording ####

As a user,

1. I would like to have a button to start audio recording.
2. I would like to have a button to stop audio recording.
3. I would like to delay the start of a recording by a specified amount of time.
4. I would like to record audio for a specified amount of time.
5. I would like to record until I manually stop the recording.
6. I would like the application to automatically write to a new file when file size limit is exceeded so that recording can continue seamlessly.
7. I would like to pause the capture process.
8. I would like to resume the capture process.
9. I would like to  adjust the volume of monitored audio (if time allows).
10. I would like to visualize audio from the selected device (if time allows).

As a developer,

1. I would like to get an audio sample from Windows.
2. I would like to get an audio sample from Mac OS X.
3. I would like to get an audio sample from Linux.

#### Editing ####

As a user,

1. I would like to trim captured audio (if time allows).
2. I would like to add audio filters (if time allows).
3. I would like to play back what was most recently recorded.
4. I would like to have a button that starts playback of recorded audio.
5. I would like to have a button that pauses playback of recorded audio.

#### Exporting ####

As a user,

1. I would like to export captured audio to a file.
2. I would like to choose the bitrate for exported files.
3. I would like to choose the bit-depth for exported files.
4. I would like to export captured audio in WAV format.
5. I would like to export captured audio in MP3 format.
6. I would like to export my audio in other formats (if time allows).
7. I would like to be able to link my Google Drive account (if time allows).
8. I would like to be able to link my Dropbox account (if time allows).
9. I would like to be able to upload captured audio to linked storage services (if time allows).

### **Non-Functional Requirements** ###

#### Architecture Requirements ####

As a developer,
    
1. I would like the build system to be modular.
2. I would like the build system to support cross-platform compilation.
3. I would like the application to work on Windows, Mac OS X, and Debian-based Linux.

As a third-party developer,

1. I would like to easily use the project’s libraries in my application.
    
#### Performance Requirements ####

1. I would like the application to have a long MTBF.
2. I would like the system audio being recorded to be compressed on-the-fly to conserve disk space and extend maximum recording duration.
                  
#### Design Requirements ####

As a developer,

1. I would like at least 70% of our code to be covered by the test suites.
    
#### Security Requirements ####

As a user,

1. I would like to be informed when the application is recording system audio.
2. I would like my application to stay up-to-date.

    
#### Usability Requirements ####

As a user,

1. I would like the installation process to be both intuitive and take less than three minutes.
2. I would like to be able to check for updates manually within the application.
3. I would like the GUI to be intuitive and straightforward.
4. I would like the CLI to provide the same feature set as the GUI.
![HulaLoop Logo](/res/logo/hulaloop-header-small.png)

# Design Document #

Team 30 : Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Neel Patel

# Table of Contents #

## **[Purpose](#purpose-1)** ##
- [Functional Requirements](#functional-requirements)
- [Non-Functional Requirements](#non-functional-requirements)

## **[Design Outline](#design-outline-1)** ##
- [High-Level Overview](#high-level-overview)

## **[Design Issues](#design-issues-1)** ##
- [Functional Issues](#functional-issues)
- [Non-Functional Issues](#non-functional-issues)

## **[Design Details](#design-details-1)** ##
- [Class Design](#class-design)
- [Class Description](#class-description)
- [Sequence Of Events](#sequence-of-events)
- [Activity Diagram](#activity-diagram)
- [State Diagram](#state-diagram)
- [UI Mockups](#ui-mockups)


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

## **Design Outline** ##
This project is an application that allows users to capture system-wide audio and export the captured data to their filesystem or cloud storage account. Our project follows a layered architecture. The user interface and backend provide different levels of abstraction with higher levels reaching down into lower levels. Each layer in the architecture forms a submodule around the functionality required to capture system-wide audio. The audio backend will access OS-level components to intercept system audio and transfer it to an OS independent interface. This layered architecture connects the UI to the backend in the same manner that a third-party developer would, ensuring the reusability of our library.
### **High-Level Overview** ###
The user interface and backend Controller form a one-to-one relationship, providing a nexus point for the distribution of audio within the application. When the audio backend receives data from the OS, it triggers a callback that runs an interface specific operation. The Windows OS, Linux OS, and Mac OS modules represent the OS dependent APIs, which relay their data to the Controller. The Controller acts a factory for the OS specific modules and facilitates the relay of information back to the interface or third-party. This layered architecture allows other applications to access the audio backend without the complication of constructing OS specific objects.

![Overview](/docs/images/high-overview.png)

1. Interface
    - Makes transport control updates to the Controller.
    - Shows visualizer and editor via a swappable card-view.
2. Controller
    - Acts as a main bridge between the UI and the audio backend.
    - Facilitates the callback triggering and passing of audio data.
3. OS Audio Interface
    - Collects and stores device information.
    - Captures system audio and routes it to the Controller.


## **Design Issues** ##
### **Functional Issues** ###
1. How should we deliver updates to the user?
	```
    Option 1: Notify user of an update and display a link to the download page
	Option 2: Automatically download and install update
    ```
    Decision: We chose to automatically download and install updates in order to simplify the update process and prevent distracting pop-ups notifying the user of updates. This also enables us to automatically push critical patches with the knowledge that any user connected to the internet will be kept up-to-date.

2. Should the user see the visualizer and the editor at the same time?
    
    ```
    Option 1: Yes, both should be put on the home screen
    Option 2: No, there should be a slider toggling the visualizer and editor
    ```
    Decision: We chose to individually display the editor and the visualizer. Putting both on the home screen would show too much information to the user at the same time, going against our simple and intuitive design philosophy. Creating a swappable interface, where only one can be displayed, gives our user an interface that looks cleaner and is more intuitive to use.

3. How can we acquire system audio from the OS and transfer it to the user interface?
    ```
    Option 1: Polling
	Option 2: Asynchronous-callbacks
    ```
    Decision: We chose async-callbacks as the main method for transferring captured audio within our application. Asynchronous-callbacks allow the application to continue running normally, only processing data when a callback is triggered by a lower level class. Polling, on the other hand, would constantly require system resources and be detrimental to the user’s experience.

4. How should the user interact with the application when CLI is executed?
	
    ```
    Option 1: Non-interactive CLI
	Option 2: Interactive CLI
    ```
    Decision: We plan to incorporate an interactive CLI where the user can command actions to the application which will respond in real-time. This method will increase functionality of the application when running in CLI mode and provide the user with greater control over the application. The interactive mode will help to ensure that the CLI provides the same functionality and usability as its GUI counterpart.

5. Should the visualizer be displayed at all times?

    ```
	Option 1: No, it should only be shown during recording
	Option 2: Yes, it should always be shown regardless of recording state
    ```
    Decision: We chose to only display the visualizer while recording. One issue with constant visualization is that it requires the consistent presence of an input device. This reliance could make our application more prone to device errors or dropouts. It could also be confusing. Showing the visualization during only the recording state is a clear signal to the user that recording has successfully started.

### **Non-Functional Issues** ###
1. Which language should our frontend be written in?
	
    ```
    Option 1: C# WPF
	Option 2: Qt Quick QML
	Option 3: GTK+
    ```
    Decision: We plan to use Qt Quick QML for our frontend. Although C# WPF can be cross-platform compiled, it is not feasible to bridge our C++ code (especially backend audio libraries) to C#. Though GTK+ is C++ based and cross-platform, its widget-based design is very heavy and does not provide the functionality we require to create a clean, customized look. Qt Quick offers intuitive styling and layouts, which enables us to quickly prototype and adapt as we iteratively reach a final design.

2. Which language should our backend be written in?

    ```
    Option 1: C#
	Option 2: C++
    ```
    Decision: Though C# has the ability to compile cross-platform, the implementation is non-standard and third-party. Furthermore, the libraries that our application requires are written in C++, and the Qt framework that we will be using does not support C#. All in all, C++ provides a wider range of support and works well with the speed-oriented, low-level programming that we plan to do.

3. How do we maintain a reliable application with a low MTTR even after a crash?

    ```
    Option 1: Provide descriptive error messages/codes on failure
    Option 2: Generate and upload crash reports automatically
    ```
    Decision: We plan to use descriptive error messages/codes. Since we are developing this application under a time constraint, automatically generating and uploading crash reports would add an additional level of complexity. Additionally, users who have no Internet access would never be able to upload logs. This led us to choose descriptive error messages which the user can search or contact us about.

4. What service should we use to host our compiled binaries?
	
    ```
    Option 1: AWS
	Option 2: DropBox
	Option 3: Github Releases
    ```
    Decision: We plan to use Github Releases to host our compiled binaries. Although AWS offers mass storage, this option is more expensive than the others. Dropbox has free storage and a simple HTTP API, however, their services limit bandwidth to 20 GB per month. Github Releases offers a solution with unlimited bandwidth, a simple API, and large binary storage. It also provides the extra capability of tying our application releases to a specific Git commit.

5. How do we compile our application on multiple platforms?
	
    ```
    Option 1: CMake
	Option 2: QMake with Qt Project file
	Option 3: SCons
    ```
    Decision: We plan to use CMake to simplify our cross-platform compilation. SCons is relatively new and not as widely supported as the other two options; we fear that this might lead to its eventual discontinuation or lack of maintenance. We discluded QMake because of its strong ties to the Qt Framework, which may be incompatible with other developer’s projects. Therefore, CMake is the perfect choice as it is open source, has a large community, and has a fantastic reputation.

6. Which audio formats should we support?
	
    ```
    Option 1: Only MP3
	Option 2: MP3 and WAV
	Option 3: MP3, WAV, and FLAC
    ```
    Decision: We chose to support MP3 and WAV audio file formats. Due to its lossy nature, supporting only MP3 would not appeal to users who need high-quality, uncompressed audio. We chose to support MP3 for those who require extended duration of lower-quality audio or need to save disk space. Though there are many other compressible formats available, MP3 is very well known and has cross-platform support. We chose to support WAV because it is a popular standard for uncompressed audio. The best of both worlds, lossless and compressed audio, is available with FLAC encoding. FLAC, however, is a niche audio format, and we thus chose not to support it for the time being.

## **Design Details** ##
### **Class Design** ###

![Class-Design](/docs/images/uml-class-diagram.png)

### **Class Description** ###
The application is broken up into a set of submodules and further refined to individual classes. The blue squares on the class diagram represent two of the distinct submodules (Audio and UI respectively). The other two are Filesystem and Transport, which are composed of some classes present on the class diagram but also others omitted as a result of the system domain model.

#### UI ####
##### Interface #####
- Contains entry point for the application.
- Initializes an instance of the Audio submodule Controller class.
- Initializes an instance of the Transport class, passing it the instance of Controller.
- Initializes either a GUI or CLI based on arguments provided.
##### GUI #####
- Holds all components (windows, buttons, combo boxes, etc.) of the application.
- Creates instance of Visualizer.
- Adds Visualizer’s audio data handler to the Controller callback list.
##### CLI #####
- CLI parses arguments to determine record countdown, record length, output file, etc.
- Executes any immediate flags or enters interactive mode if none present.
##### Visualizer #####
- Generates a visual representation of the audio received from the Controller callback.
- Performs necessary calculations to create a custom animation.
Audio
##### Controller #####
- Created when UI is launched.
- Creates instance of OSAudio class for the appropriate OS architecture.
- Adds its own audio data handler to the OSAudio callback list.
##### OSAudio #####
- Populates a list of Device objects based on the OS’s available devices.
- Sets active input and output Devices.
- Initializes device read/write streams upon request.
- Holds a buffer of a specified size during recording.
##### Device #####
- Represents a wrapper around the functionality of OS-level endpoints.
- Holds a unique identifier.
- Holds a device type that determines what capabilities the device has.

#### Transport ####
##### Transport #####
- Created by the Interface class on application startup.
- Maintains the state of the application (recording, playing, etc).
- Is composed of an instance of Recorder, Playback, and Monitor.
- Delegates requests for record, play, pause, etc. to their respective objects.
##### Recorder #####
- Creates an instance of the Export class with respect to a temporary file.
- Writes recording audio to temporary file via instance of Export class.
##### Playback #####
- Handles all playback operations.
- Maintains the playback position for play, pause, and jump operations.
##### Monitor #####
- Route incoming audio to an output device so that it is audible during record.
- Maintains the mute state of the monitor output.
#### Filesystem ####
##### Export #####
- Wrapper for an exported audio file of a specified encoding.
- Contains an instance of the Encoder class for encoding data in different formats.
- Writes the appropriate header to the audio file upon instantiation.
- Encodes and appends to the file as audio data is received.
##### Encoder #####
- Encodes and decodes buffers of PCM audio data to and from other audio formats.
##### File #####
- Holds OS independent wrapper of a file path and any associated metadata.

### **Sequence Of Events** ###
#### Sequence of Events when the user starts the application ####

![SOE-Start](/docs/images/soe-start-app.png)
___

#### Sequence of events when use starts recording ####

![SOE-Rec](/docs/images/soe-start-rec.png)
___

#### Sequence of events after recording ends ####

![SOE-End](/docs/images/soe-end-rec.png)
___

### **Activity Diagram** ###

![Activity-Diagram](/docs/images/activity-diagram.png)

### **State Diagram** ###

![State-Diagram](/docs/images/transport-state-machine.png)


### **UI Mockups** ###
- Main Application

![Main-UI-Eq](/docs/images/main-ui-eq.png)

![Main-UI-Edit](/docs/images/main-ui-edit.png)

- Export UI

![Export-UI](/docs/images/export-ui.png)

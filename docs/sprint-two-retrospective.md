![HulaLoop](/res/logo/hulaloop-header-small.png)

# **Sprint 2 Retrospective** #
Team 30: Aneesh Gokhale, Emanuel Pituch, Jack McKernan, Meriem Bounab, Neel Patel

## **What Went Well?** ##
During this sprint, we managed to complete most of the functionality that we had planned. This included the addition of the CLI, the creation of the updater/launcher module, the ability to save audio to a file, and the improvement of the visualizer to respond to real audio data. The modular class structure that we laid out in our initial design simplified a lot of the implementation this sprint, especially when adding the CLI. Because of our strict adherence to our design architecture and abstraction, the backend that we created with the GUI last sprint was one-hundred percent reusable with the CLI this sprint. This strategy helped keep our duplicate code down and maintainability high. We hit our goals of having above 70% code coverage, a high CodeFactor score, and well-documented code.

### **User Story #1:** ###
As a user, I would like to choose an input device to record audio from.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Emanuel |
| 2 | Make sure the device has the proper capabilities | 1 | Emanuel |
| 3 | Ensure valid sampling and bit rates are available | 2 | Emanuel |

#### Completed: ####
All operating systems that we support are capable of switching input devices. The UX for switching differs slightly per system and is something we plan to improve next sprint. When a device switch is signaled, the backend joins all capture threads and spawns new ones when appropriate. When the user selects the device we make sure that the device has the proper capabilities such as the valid sample rate and bit depth.

### **User Story #2:** ###
As a user, I would like to choose an output device to record audio from (loopback).

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Emanuel |
| 2 | Make sure the device has the proper capabilities | 1 | Emanuel |
| 3 | Ensure valid sampling and bit rates are available | 2 | Emanuel |

#### Completed: ####
All operating systems that we support are capable of switching output devices. The UX for switching differs slightly per system and is something we plan to improve next sprint. When a device switch is signaled, the backend joins all playback threads and spawns new ones when appropriate. When the user selects the device we make sure that the device has the proper capabilities such as the valid sample rate and bit depth.

### **User Story #3:** ###
As a user, I would like to record audio for a specified amount of time.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a bridge between the Transport and the Record class | 3 | Emanuel |
| 2 | Register Record class with the Controller to receive audio data | 2 | Emanuel |
| 3 | Unregister with the Controller class when the timer reaches the desired duration | 2 | Emanuel |
| 4 | Unit Test - Verify that state between the Transport and Record class is synced | 2 | Emanuel |
| 5 | Unit Test - Verify the registration and deregistration of the Record class with the Controller class | 2 | Emanuel |

#### Completed: ####
Transport and Record are linked via an aggregational association. Using the memory management and buffering system that we designed this sprint, Record can register itself with Controller via Transport and receive audio data. Unregistering occurs via the same path. A few unit tests were created for these classes, but we relied more heavily on manual testing this sprint as it was more convenient and more easily verifiable.

### **User Story #4:** ###
As a user, I would like to record until I manually stop the recording.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Remove self from callback list when stop button is triggered | 1 | Neel |
| 2 | Design and implement UI transport control state machines for stop button | 3 | Neel |
| 3 | Update other UI buttons based on the state machine | 1 | Neel |
| 4 | Unit Test - Verify that the callback is removed when the stop button is pressed | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks and states | 2 | Neel |

#### Completed: ####
We created state machines for the various Transport and UI states. Using the memory management and buffering system that we designed this sprint, Record can register itself with Controller via Transport and receive audio data. Unregistering occurs via the same path. A few unit tests were created for these classes, but we relied more heavily on manual testing this sprint as it was more convenient and more easily verifiable.

### **User Story #5:** ###
As a user, I would like to pause the capture process.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Remove self from callback list when pause button is triggered | 1 | Neel |
| 2 | Design and implement UI transport control state machines for pause button | 3 | Neel |
| 3 | Update other UI buttons based on the state machine | 1 | Neel |
| 4 | Unit Test - Verify that the callback is removed when the pause button is pressed | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks | 2 | Neel |

#### Completed: ####
We created state machines for the various Transport and UI states. Using the memory management and buffering system that we designed this sprint, Record can register itself with Controller via Transport and receive audio data. Unregistering occurs via the same path. A few unit tests were created for these classes, but we relied more heavily on manual testing this sprint as it was more convenient and more easily verifiable.

### **User Story #6:** ###
As a user, I would like to resume the capture process.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add self to the OS callback again when record button is clicked again | 1 | Neel |
| 2 | Design and implement UI transport control state machines for record button | 3 | Neel |
| 3 | Design and implement UI transport control state machines for record button | 1 | Neel |
| 4 | Unit Test - Verify that the callback is added back to the callback list | 1 | Neel |
| 5 | Unit Test - Verify the transport controls state machine by testing all possible button clicks and states | 2 | Neel |

#### Completed: ####
We created state machines for the various Transport and UI states. Using the memory management and buffering system that we designed this sprint, Record can register itself with Controller via Transport and receive audio data. Unregistering occurs via the same path. A few unit tests were created for these classes, but we relied more heavily on manual testing this sprint as it was more convenient and more easily verifiable.

### **User Story #7:** ###
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

#### Completed: ####
The visualizer now automatically adjusts to the window’s width. The FFT algorithm converts the time-domain audio data to various bins in the frequency domain. These are displayed in the UI and are updated in real-time. The visualizer adheres to the play/pause button state by starting and stopping the animation respectively. The dancing line dances above the bars. There will eventually be an option to choose one of these visualizations (or both).

### **User Story #8:** ###
As a user, I would like to export captured audio to a file.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create a button that allows the user to export the audio file | 1 | Meriem |
| 2 | Create a dialog box that shows up when the export button is pressed | 2 | Meriem |
| 3 | Create a cross-platform SaveFileDialog that allows for renaming files and saving to a specific directory | 5 | Emanuel |
| 4 | Check for errors when saving the file | 1 | Emanuel |
| 5 | Unit test: Verify error handling and correct file saving behavior | 3 | Meriem |

#### Completed: ####
When a user is done recording, a button that allows the user to export the recorded audio to disk is enabled. When the user presses the button, a save-file dialog appears, allowing the user to browse their filesystem and save the file anywhere their heart desires. If the file already exists or some other error occurs, the user is notified.

### **User Story #9:** ###
As a user, I would like to export captured audio in WAV format.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Integrate WAV encoding libraries per platform | 4 | Neel & Jack |
| 2 | Acquire all raw temporary audio files and merge into single audio buffer | 2 | Neel |
| 3 | Encode the PCM audio buffer into WAV format | 3 | Neel |
| 4 | Use the cross-platform SaveFileDialog to store the WAV file to a specific local directory | 3 | Neel |
| 5 | Unit Test - Verify WAV encoding integrity | 2 | Neel |

#### Completed: ####
We chose libsndfile as our WAV encoding library. Packages for both Linux and macOS were already available. On Windows, we built from source and plan to distribute the DLL along with our application. The Record class maintains a list of all temporary files for each record session; these files are then merged via the Export class to the final WAV file. We relied on manual testing to verify the integrity of the WAV file and plan to implement a checksum test next sprint.

### **User Story #10:** ###
As a third-party developer, I would like to set the device from which I capture audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Notify the backend of the device switch | 1 | Jack |
| 2 | Initiate new callback threads once Controller receives device switch from third-party application | 1 | Jack |
| 3 | Expose a subset of our Controller API to third-parties for switching devices | 1 | Jack |
| 4 | Create documentation detailing how third-party developers should access device lists and use the exposed API | 4 | Meriem |

#### Completed: ####
All operating systems that we support are capable of switching output devices. When a device switch is signaled, the backend joins all playback threads and spawns new ones when appropriate. The public headers for our hlaudio library expose the necessary functions. The Doxygen page https://hulaloop.readthedocs.io/en/sprint-2-review/public contains the public API.

### **User Story #11:** ###
As a developer, I would like my application to have minimal memory leaks.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Determine memory management strategy | 5 | Emanuel, Jack, Neel |
| 2 | Implement the memory management strategy | 7 | Emanuel, Jack, Neel |
| 3 | Document the strategy for internal use and third-party developers | 3 | Emanuel, Jack, Neel |
| 4 | Create valgrind tests to quantitatively measure memory leaks | 4 | Emanuel, Jack, Neel |

#### Completed: ####
Throughout the sprint we greatly reduced memory issues that we experienced during the first sprint. We designed a buffering strategy that allowed us to very simply distribute audio within our application and implemented it for each of our targeted operating systems. The Doxygen page https://hulaloop.readthedocs.io/en/sprint-2-review/mem-management contains the memory management documentation for third-party developers. Memory leaks and other problems were quantified and eliminated by adding Valgrind tests to the CTest suite.

### **User Story #12:** ###
As a developer, I would like at least 70% of our code to be covered by the test suites.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Act on results of code quality, static analysis, and code coverage to improve existing code | 15 (1 per week) | Aneesh, Emanuel, Jack, Meriem, Neel |
| 2 | Add additional unit tests to cover more code | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |
| 3 | Remove dead code as indicated by missing coverage | 5 | Aneesh, Emanuel, Jack, Meriem, Neel |

#### Completed: ####
Throughout the sprint, this goal motivated us to add unit tests in order to maintain our code coverage standards. We spent time reviewing reports from both the runtime and static analysis tools, removing dead code and adding additional tests for weak areas.

### **User Story #13:** ###
As a user, I would like to be informed when the application is recording system audio.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Research system tray icons | 1 | Aneesh |
| 2 | Create a cross-platform system tray icon | 3 | Aneesh |
| 3 | Update system tray icon based on recording state | 1 | Aneesh |
| 4 | Send a balloon message to user notifying recording | 2 | Aneesh |

#### Completed: ####
We implemented a cross-platform system tray icon with a custom menu with options such as record, stop, minimize, maximize, check for update, settings, and quit. When a user triggers a record event, a balloon message is displayed to the user, and the system tray icon is updated to match the transport state.

### **User Story #14:** ###
As a user, I would like my application to stay up-to-date.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Start Updater process when launching the application | 1 | Aneesh |
| 2 | Create Updater GUI with progress bar and text labels | 3 | Aneesh |
| 3 | Query GitHub Releases API to determine if an update is available | 2 | Aneesh |
| 4 | If an update is found, download the release asset | 1 | Aneesh |
| 5 | Unpack downloaded ZIP containing necessary application files | 3 | Aneesh |
| 7 | Remove all older instances of application files | 2 | Aneesh |
| 8 | Launch updated HulaLoop application | 1 | Aneesh |

#### Completed: ####
We created a launcher GUI with a progress bar and status text labels. When the updater is started, it queries the GitHub Releases API to check if an update exists. If an update is found, the user is asked if they want to download and install the update.

### **User Story #15:** ###
As a user, I would like the installation process to be both intuitive and take less than three minutes.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 2 | Customize install process | 7 | Aneesh |

#### Completed: ####
We created an installer/uninstaller using the Qt Installer Framework instead of CMake/CPack. The installer/uninstaller was customized using XML files and custom JS scripts along with image resources. In the next sprint, we plan to integrate the process of creating installers/uninstallers with CPack.

### **User Story #16:** ###
As a user, I would like to be able to check for updates manually within the application.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Add styled check update button to layout manager | 1 | Aneesh |
| 2 | Notify user if an update is available | 1 | Aneesh |
| 3 | Start update process | 1 | Aneesh |

#### Completed: ####
Created a styled button in the GUI that checks for updates and prompts the user accordingly. If the user chooses to download/install the update, the specified process takes over.

### **User Story #17:** ###
As a user, I would like the CLI to provide the same features as the GUI.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Create an interactive CLI with a set of commands | 10 | Jack & Meriem |
| 2 | Create a non-interactive CLI using a set of flags | 5 | Jack & Meriem |
| 3 | Support transport controls and export | 7 | Jack |
| 4 | Add a man page/help dialog displaying all flags and commands | 5 | Jack |

#### Completed: ####
We completed both the interactive and non-interactive CLIs. Since we adhered to our design architecture so well the first sprint, it was relatively simple to hook this UI into the backend used by the GUI. We used help2man to automate the creation of the man page from the help text output by ```./hulaloop-cli -h```.

## **What Did Not Go Well?** ##
Most of this sprint went very well. All of the user stories and tasks that we planned were completed in one way or another. The struggle that we had, however, was consistency across our targeted platforms. Since most of us develop solely on one platform, there was a lack of testing on the other platforms until the end of the sprint. This led to a few features not working as well on the platforms that they were not developed on. We also ended up relying more heavily on manual testing this sprint than previously.

### **User Story #14:** ###
As a user, I would like my application to stay up-to-date.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 6 | Warn user about running instances of HulaLoop if update is ready to install | 1 | Aneesh |

#### Not Completed: ####
The application warns the user about running instances on Windows, while Linux and Mac OS X will allow the application to continue running while the new version of the application is installed.

### **User Story #15:** ###
As a user, I would like the installation process to be both intuitive and take less than three minutes.

| Task # | Description | Time (hrs) | Owner |
|:------:|:-----------|:----------:|:-----:|
| 1 | Generate cross-platform installers using CPack | 3 | Aneesh |
| 3 | Create cross-platform uninstallers within the CPack generators | 3 | Aneesh |

#### Not Completed: ####
The installer/uninstaller that were generated were only created on Windows. In the next sprint, we plan to integrate the process of creating installers/uninstallers with CPack which will further automate the generation for all supported platforms.

## **How Should We Improve?** ##
This sprint was just as excellent a learning experience as the previous sprint, but was also much more satisfying. Since we had a working product at the beginning of the sprint, it was exciting to add new features and be able to try them out in the “real” application. We discovered that this excitement is also dangerous. Our primary issues with this sprint were a lack of cross-platform consistency and improved, but still subpar, time management.


With the great excitement of implementing new features, came great responsibility. Since each of us is developing on a different platform, the excitement of making a feature work on “your” platform might not translate to that feature working on everyone else's platform, especially since a majority of the features that were implemented this sprint all function differently on each targeted platform. A greater focus on cross-platform development and testing will lead to less headaches next sprint, especially at the end of the it.


Last sprint, we vowed to improve our time management. This sprint started out promising, but we still failed to follow through with the complete balance of work. Our estimation of work was closer to the actual required time, but we still underestimated the time required for some tasks this sprint. Next sprint, we plan to estimate even more wisely and try to balance our load evenly throughout the sprint.
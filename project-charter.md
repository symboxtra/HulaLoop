# Team 30 Project Charter #

### **Project Title:** ###
HulaLoop

### **Team Members:** ###
Aneesh Gokhale, Emanuel Pituch, John McKernan, Neel Patel

### **Problem Statemenet:** ###
There are many technologies for capturing and routing system-wide audio. 
These technologies vary widely by operating system and can be difficult for developers to interact with and for users to understand. 
While there are existing technologies that provide a loopback interface for capturing this audio, their features are limited, and none are completely cross-platform. 
By delivering a library for developers and an easy-to-use UI for average consumers, anyone should be able to visualize, capture, and use their system audio as they see fit.

### **Project Objectives:** ###
- Develop a cross-platform application to standardize and streamline the loopback process for both users and developers
- Create an intuitive user interface for simple audio processing (EQ, clipping, etc.)
- Design an export process that allows the user to save their recorded audio to various locations (Google Drive, Dropbox, local storage, etc.)
- Write a well documented API to ensure maintainability and usability of the project

### **Project Deliverables:** ###
- Front-end Qt Quick application that allows users to visualize and export their audio
- Command line interface supporting a similar set of features to the UI
- C++ backend that processes audio and interacts with the OS
- Use WASAPI, JACK, ALSA, and CMake to develop an extensible, cross-platform library for other developers to include in their projects (Windows, OSX, Linux)
- Documented API

### **Project Stakeholders:** ###
- Users - Content creators, music listeners, or other developers looking to capture and process system audio
- Developers - Aneesh Gokhale, Emanuel Pituch, John McKernan, Neel Patel
- Project Manager - Shane Li
- Project Owners - Aneesh Gokhale, Emanuel Pituch, John McKernan, Neel Patel

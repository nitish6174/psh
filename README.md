# psh
**Shell supporting basic commands made with C**

### Features
- Changes shell directory with ```cd``` command
- Exits shell with ```exit```
- ```help``` command shows help
- ```history``` command shows history of last 10 commands entered
- ```!!``` runs last entererd command. ```!3``` runs 3rd command in history and so on
- Implementation of alias by ```alias``` command (addition, deletion and listing of aliases)
- Supports all normal file & directory commands: ```pwd```, ```ls```, ```mkdir```, ```touch```, ```rm```, ```cp```, ```mv``` etc
- Executes all bash commands like ```clear```, ```grep``` . . .
- Multiple piping of commands
- Both input and output redirection

### Usage
1. In the root directory, enter ```make``` command in terminal to make the psh program using ```makefile```
2. Now enter ```./psh``` in terminal to launch the psh program

### Known issues
- Piping doesn't work with history command


### Contribution
* 140101023 - Gaurav Manchanda
  - Alias
  - Piping
  - Shell Basic Functionality

* 140101039 - Maulik Patni
  - History
  - Redirection
  - Shell Basic Functionality
* 140101045 - Nitish Garg:
  - Alias
  - Parsing Commands
  - Redirection 
* 140101063 - Sambhav Kothari:
  - History 
  - Piping
  - Reading and Parsing Commands

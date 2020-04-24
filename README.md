# Blacktop
When cloning this project you need to make some additional git commands to get the unity unit test framework and cmock mock framework

cd blacktop

git submodule init

git submodule update


# Test scripts

There is a test script in the blacktop directory that will run unit tests every time that you save any of the source code. It will run all the unit tests for all subdirectories.

In order to use it you need to install a python tool called when-changed. The command is:

pip install when-changed

Run it in the blacktop directory using the following command:

./watchtests.sh

There is also a system test script in the test folder that can be ran using the following command:

./runtest.sh

# Directories

license: contains the relevant license files for this project

standards: contains the coding standards that should be followed for this project

src: contains the source code and appropriate tests for this project

doc: contains the documentation for this project

spikes: contains spikes that have been done to understand how this project will work

hw: contails kicad files defining the hardware design

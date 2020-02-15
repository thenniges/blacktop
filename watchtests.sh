 #!/bin/bash

# this script sets up 'when-changed' to watch the test directory and
# rerun all unit tests if there are any changes

# sourcedirectory = "./src"
# testdirectory= "../Unity"

when-changed -r -1 "./src" -c bash runtests.sh

# when-changed -r -1 ${sourcedirectory} -c bash runtests.sh

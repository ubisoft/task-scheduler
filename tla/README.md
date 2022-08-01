# What is TLA+
It is a language which allows to write specifications for systems and algorithms using a mathematical-like language which is extended with bits of programming languages and temporal logic.

A specification can be verified if it is formally correct in a sense that its state machine has no deadlocks, infinite loops, and other logical bugs. That gives a much deeper assurance than any unit tests that the verified algorithm is truly correct.

# How to run
One way is to install a TLA+ GUI tool called "Toolbox" and verify a spec in it. The other way is via the command line.

* Download the `tla2tools.jar` file from the official repository: https://github.com/tlaplus/tlaplus/releases#latest-tla-files.
* Install this `.jar` file somehow. For instance, on Mac it can be saved into `/Library/Java/Extensions/`.
* Define an alias for bash: `alias tlap='java -XX:+IgnoreUnrecognizedVMOptions -XX:+UseParallelGC -cp /Library/Java/Extensions/tla2tools.jar tlc2.TLC'` in your `.bash_profile` file or wherever is preferred. Replace the path with the place where the file is really stored, if it is different from this example.
* In a console do: `tlap <spec_name>.tla`.

In `.cfg` files there are parameters which can be tweaked to verify specific corner cases explicitly. But keep in mind that increasing the parameters too much often leads to a state machine explosion (verification can run for hours, days) without actually covering more.

There are options which can be seen via `tlap -h` and in a bit different (somewhat extended, somewhat reduced) form here: https://lamport.azurewebsites.net/tla/current-tools.pdf.

There is no good documentation for syntax and builtin functions and modules except the video course from Leslie Lamport and the book *'Specifying Systems'*. Sometimes bits of useful info can be found here: https://learntla.com/tla. Also can ask questions in the official Google Group: https://groups.google.com/g/tlaplus.

# Links
* Latest TLA+: https://github.com/tlaplus/tlaplus/releases#latest-tla-files
* Official doc: https://lamport.azurewebsites.net/tla/current-tools.pdf.
* Unofficial doc: https://learntla.com/tla
* Google Group: https://groups.google.com/g/tlaplus.

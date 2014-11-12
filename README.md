RE2DFA
=======

In order to match a regular expression (RE), it may first be converted into a deterministic finite automaton (DFA). Matching the RE to an input string is then a matter of processing each character of the input string, making state transitions according to the character. If the DFA is at an accept state once at the end of the
string is reached, then the RE matched.

For example, the regular expression `ˆ(a+b+(c|d)+)+$` can be represented by the following automaton, assuming a string alphabet of `{a, b, c, d}`.

![RE example DFA](./assets/example_dfa.png)

The matching process is inherently sequential, since we need to process the string one character at a time. It may be parallelized, however, through the use of an optimistic approach.

Assume we have `1` normal thread, and n optimistic threads. We divide the input string into `n + 1` pieces. The normal thread gets the first piece of the string, and performs matching as above.

The n optimistic threads each perform matching on their own portion of the string, but since they are not sure what state the DFA will be in to start with, they simulate matching from every possible state simultaneously. For instance, in the above example, an optimistic thread would be checking its fragment 4 times, assuming it started in state `0`, `1`, `2`, or `3` (you do not really need to model starting in the reject state, as reject has no transitions out). In effect, each optimistic thread computes a mapping from each possible state of the DFA to the resulting state for the associated input fragment. Note that this means the optimistic threads might do more work than the normal thread, even on the same size input string fragment.

Once the normal thread reaches the end of its input fragment `i`, it looks at the mapping produced by the thread handling the next fragment `i + 1`. It knows the ending state of `i` in the DFA, and so can use the optimistic map to compute the resulting ending DFA state of the `i + 1` fragment. This repeats until the
matching process is completed for the entire string.

The implementation is done in OpenMP on top of C/C++. The example DFA shown above is hardcoded (I did not do any RE→DFA conversion), and it includes a function that generates a (very long) string that would match; an example code fragment is provided. The simulation accepts a command-line parameter for controlling the number of optimistic threads, and runs the test 10 times, timing the total matching time (excluding the string construction time). Data is shown for 0–3 optimistic threads.

Here is an explaination of the results in relation to the number of processors in the test hardware. Let's make sure the solution demonstrates speedup for some non-0 number of optimistic threads!



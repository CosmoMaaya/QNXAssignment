Timers (tick tock, tick tock)
=============================

The timers.c file represents a very simple timer framework which allows you to
setup event handling based on a timeout.  The basic primitives allow the
programmer to:
	-allocate or free timers
	-arm/disarm timers
	-set attributes and timeout for a timer

Currently, the self contained example will create timers up to the maximum
(currently defined as 10) with random timeout values from up to 20 seconds.
Each timer will eventually expire and the associated call-back function executed.

The assignment is to identify as many weaknesses or bugs in the framework as you
can find.  You are welcome to identify any type of issue you feel is relevant.
Anything from logic errors to algorithmic limitations are fair game for
identification.  Just provide a illustrative text description of each problem and
be prepared to discuss your findings.  Let us know why you believe the problem(s)
exists and how you might go about addressing them in the code.

You are not required to run or write code but you may find it easier to understand,
debug or fix the framework (if needed).  If you have any issues building the source
or running the example, please do not hesitate to contact.


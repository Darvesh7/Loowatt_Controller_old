# Controller Unit

## Motor and Encoder Spec
Motor: ETONM MOTOR ET-WGM58AE-1220.6 DC12V  <br />
Encoder: ET-MY37 Two channel hall effect encoder

## Notes on encoder
Encoder V2 - SU- 02 (Bebe, 3D printed SU’s) <br />
Using a motor that later failed and was not upto the job so had to be replaced. <br />
30 pulses on start up <br />
16 ticks <br />
735 pulses per flush <br />
735 * 100 / 815 = 90mm <br />


## Background
Loowatt’s current system is currently based around a fleet of toilet trailers. Key to each toilet’s functionality is a mechanical sealing unit, which transports human waste from the toilet bowl to the waste barrel, via mechanical rollers and biofilm, forming an odour-proof mechanical seal behind it.

The sealing unit’s rollers are driven by a DC motor with encoder feedback (product link). The motor runs forward when a toilet users presses the flush button.

A controller unit mediates this interaction. It has the following functions:
* Observe when a user presses the flush button.
* Instruct the motor to move forwards and backwards, via a motor driver.
* Ensure the motor turns the instructed number of revolutions, via its built-
in encoder.
* Remember how many times the motor has turned, and infer how much
biofilm has passed through the sealing unit.
* Know when film is running out (e.g. LED, communicate to attendant unit,
described below)
* Allow the attendant to manually run the motor backward and forward
* When the attendant replaces the film, reset the internal memory of film
length.
* Provide audio feedback (beep) when attendant has successfully
completed an operation.

All controller units in the trailer (2, in this case) are attached to a single attendant unit per trailer. It has the following functions:
* Accept communications from the controller units, telling it how many flushes remain.
* Display remaining flushes on an LCD display.
* Communicate if a toilet is running low on biofilm via the LCD display.
* Connect to an off-the-shelf IoT web-connected device (product link) to
upload this data to a web server.
* A web-app (service link) accessible by a web-connected tablet can display
the remaining flushes and low biofilm warnings for all connected attendant units (i.e. all trailers). The app should also keep a history of usage for later analysis.



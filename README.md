Programmable Tactile Stimulator

A two-channel stepper-motor tactile stimulator for animal neuroscience experiments. Two independently driven stepper motors carry probes that can be swept against a subject. Configured for left/right whisker stimulation — either on command over serial or on a TTL trigger from an external acquisition/behavior system.

Each stimulus is a single out-and-back sweep: the motor rotates num_steps in one direction, optionally holds, then returns to its original position. Three stimulus types are available:

There are two ways to trigger:
1. Serial commands — send a single character over USB serial:
  l → left stimulus
  r → right stimulus
  b → both
2. Hardware TTL triggers — pins 10 (left) and 12 (right). 

Requires the TeensyStep library,

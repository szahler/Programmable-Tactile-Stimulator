Programmable Tactile Stimulator

A two-channel stepper-motor tactile stimulator for animal neuroscience experiments. Two independently driven stepper motors carry probes that can be swept against a subject. Configured for left/right whisker stimulation — either on command over serial or on a TTL trigger from an external acquisition/behavior system.

What it does

Each stimulus is a single out-and-back sweep: the motor rotates num_steps in one direction, optionally holds, then returns to its original position. Three stimulus types are available:

Two ways to trigger

Serial commands — send a single character over USB serial:

l → left stimulus
r → right stimulus
b → both

Hardware TTL triggers — pins 10 (left) and 12 (right). The loop polls both pins and, on detecting a rising edge, waits 1 ms so a near-simultaneous partner pulse can be caught; if both lines are high it fires the bilateral stimulus instead of two separate ones. After firing it blocks until the trigger line(s) go low again, so one pulse produces exactly one stimulus regardless of pulse width.

Requires the TeensyStep library,

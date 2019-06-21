---
title: Project Actuator
author: Corridore407
date: Tuesday, June 18, 2019
layout: post
---
## Introduction 
It all began one night. I was on a discord call with a buddy and we start discussing flight sticks for playing the game [Elite Dangerous](https://www.elitedangerous.com). Some Google searches spiked my interest in having a set up for the game that had a ton of buttons to press. We joked about having to press five or six buttons, flip some switches, and then smash the big red button to jump to light speed. I thought of Star Wars, watching [Han and Chewie](https://www.youtube.com/watch?v=j0GZ3qSV9s0) frantically preparing the Millenium Falcon to jump away from the Imperial Star Destroyers. The idea was set: I was going to build a panel of buttons and switches to nerd out my experience of playing a game and flying around in space.

Some further online research led to [this](https://imgur.com/a/DyQZL) project. While truly beyond my intended scale, and overkill for my purposes, this project was very inspirational. Next, I stumbled upon this [example](https://www.instructables.com/id/How-to-Make-a-Custom-Control-Panel-for-Elite-Dange/) that seemed much more like my scale of project.

My plan began to fall into place.
1. Buy some buttons and switches.
1. Wire them into an Arduino micro-controller.
1. Program the Arduino to act as a keyboard.
1. Feel like I'm actually in a spaceship!

And thus I began. I dug out my old Arduino Uno starter set that was gifted to me a long time ago. Step 1, figure out how to turn this into a keyboard. Step 1A, figure out how to have the Arduino detect the press of a button. Step 1A:Part 1, go through the tutorial booklet from the starter set for the 3rd time.

![Cover image of Project Actuator](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/panel.jpg)

![Player POV](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/pov.jpg)

## Background
A bit of background on myself. I am a software engineer focusing on the front end of enterprise web applications. I spend most of my time writing [React](https://reactjs.org/) components to make buttons appear on a screens. I studied Computer Science in college, during which I took 1 electrical engineering course. I am quite experienced at writing software and I know the **bare** minimum about hardware. 

## Setup
Alright, so back to Google. At this point, I knew that I could create a circuit that would let the Arduino detect when a button was pressed. And I knew that I wanted to output a keystroke (or keystroke style signal) from the Arduino to the pc. The internet helped me understand what that might take. 

(If you aren't interested in the technical details, feel free to skip to the next major heading. All you need to know is: "button matrix", and "keyboard mode vs. Arduino mode")

From [this](https://www.instructables.com/id/How-to-Make-a-Custom-Control-Panel-for-Elite-Dange/) example (mentioned above), I decided to use a button matrix to wire the actuators into the Arduino. I'll let the experts explain the details but here's the basics. Buttons are arranged/wired in a row and column configuration, i.e. each button corresponds (is connected to) exactly 1 row and 1 column of the matrix. The software on the Arduino will send signals through the rows in sequence. After powering a row, it will read the column outputs. If a button is being pressed, that column will signal so. Thus, through the combination of "which row is powered" and "which column is outputting power", the Arduino can detect which button was pressed. There are some important details around signal feedback, but I'll touch on those when we discuss the actuators themselves. 

I was fortunate enough to find an Arduino-ready library that was designed to work with a matrix style keypad. This enabled me to abstract much of the specifics around signal processing. Many thanks to Mark Stanley and Alexander Brevig as you are one of the many reasons this project did not fail. 

The next thing I explored was how to send keystroke signals to a PC. [This](http://mitchtech.net/arduino-usb-hid-keyboard/) article was my primary resource for understanding how to do it, but it was rather confusing to me for a while, so I'll explain it in my own words in case that helps you. 

The goal is to get the computer to recognize the Arduino device as a HID (human interface device). The Arduino Uno does not natively support that behavior. Fortunately, the smart minds of the internet have shared how someone can make that possible. The Arduino Uno has two micro-controllers, one that gets programmed by the Arduino IDE in the "common" Arduino way, and the "tell the computer this is a programmable Arduino and let folks program it to do cool things" chip (aka. atmega32). This second chip can also be reprogrammed and that is exactly what needs to happen in this case.

Using a piece of software called [flip](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/FLIP), you can reprogram the atmega32 to behave like a keyboard. You can also "flip" it back to the standard "this is an Arduino" behavior. To do this, you need the flip software and the "compiled" binary of the desire behavior. Since the Arduino Uno is a few years old, the links I found to these files were all broken. After some internet sleuthing, I found links to those files [keyboard](https://github.com/Anarch157a/Elite-Dangerous-Keypad/blob/master/Arduino-keyboard-0.3.hex) and [arduino](https://github.com/arduino/ArduinoCore-avr/blob/master/firmwares/atmegaxxu2/arduino-usbserial/Arduino-usbserial-uno.hex). The next step was to figure out how to send a signal that would behave like a keystroke. That's where the above mentioned article helped. 

For anyone using this as a guide or tutorial, read this:
How to use [flip](https://www.microchip.com/DevelopmentTools/ProductDetails/PartNO/FLIP) when programming an Arduino Uno to behave like a keyboard (HID):

What you will need: Arduino Uno, USB cable, flip software, Arduino atmega32 keyboard firmware, Arduino atmega32 Arduino firmware, a pin jumper/small segment of wire, and patience.

This process was most clearly explained in the comments of this article, so full credit goes to them.

* When you want to upload changes you've made to the Arduino source code (via the Arduino IDE) you will need the Arduino in Arduino mode
* when you want to send keystroke signals to your computer via a programmed Arduino, you will need the Arduino in Keyboard mode
* to change the firmware of the AtuMega32, you will need the board in DFU mode
* once you flash the firmware, you will need to plug/power cycle the board. My practice was to disconnect the usb cord from the board and then reconnect it.
* to get the board into DFU mode, you will need to connect two of the pins. See image below.

![DFU mode pins](http://www.fedevel.com/welldoneblog/wp-content/uploads/2015/04/Arduino-Link-ICSP1-Pins-5-6.png)

If you are often changing your source code and trying to test it as a keyboard, you will be doing this a lot.
* trigger DFU mode
* use flip to flash the Arduino mode firmware
* unplug and plug the board.
* upload changes via the Arduino IDE
* trigger DFU mode
* use flip to flash the keyboard mode firmware
* unplug and plug the board

I opened the Windows device manager and would confirm the board was being detected as a HID or as a COM port Arduino. 

To reduce the number of times you have to do this cycling, check out the Serial Monitor tool in the IDE, it will allow you to see the output of the board while it is in Arduino mode. The keystrokes themselves are not the same as when it's in keyboard mode, so an "a" character won't display as an "a" in the Monitor tool, but you should be able to get an better indicator that your wiring and code is working as expected before you flash the firmware and such. 

## Skip to here
The example in the article used the Serial API to send signals on a port to the computer. That was easy enough to follow and I recreated a simple program to detect a button press, and send an "a" character. Then I stepped up my game and added a second button that would send a "b" character. This is what that program looked like:

```c

uint8_t buf[8] = { 0 };   /* Keyboard report buffer */

int state1 = 1;
int state2 = 1;

void setup() {
  Serial.begin(9600);
  pinMode(3, INPUT);
  pinMode(4, INPUT);

  digitalWrite(3, 1);
  digitalWrite(4, 1);

  delay(200);
}

void loop() {
  state1 = digitalRead(3);
  state2 = digitalRead(4);

  if (state1 != 1) {
    buf[2] = 4; // character
    Serial.write(buf, 8); // Send keypress
    releaseKey();
  }

  if (state2 != 1) {
    buf[2] = 5;
    Serial.write(buf, 8);
    releaseKey();
  }
}

void releaseKey() {
  buf[0] = 0;
  buf[2] = 0;
  Serial.write(buf, 8); // Release key  
}
```

And this is what would be output to the computer when I pressed button1: "aaaaaaaa"; and when I pressed button2: "bbbbbbbb". This was not me holding the button down, but pressing and releasing it as quickly as I could. The loop function of the Arduino was cycling too fast. I needed to figure out some debouncing or something. But that could be done later, the fundamentals were proven.

## Flight Stick 
At this point, I had figured out the fundamental pieces I needed to get this system in place. The next question I had: "what buttons do I need/want?". That was only answerable by exploring the game and the other input devices. So began the long shopping experience of flight sticks... Long story short (too late), I decided to go with a H.O.T.A.S. system with the most buttons and switches. The [Logitech X56](https://www.logitechg.com/en-ca/products/space/x56-space-flight-vr-simulator-controller.html#product-tech-specs) (formerly by Saitek). It took me a minute, but I was able to configure it to my keybindings I decided on and give it a test drive. This allowed me to figure out what functions I wanted on the Hotas system and which ones I wanted on the switch panel.

![Logitech x56](https://www.logitechg.com/content/dam/gaming/en/products/x56/x56-feature-6-desktop.png.imgw.1888.1888.jpeg)
Look at all them buttons and switches!

## Design the board
Well, back to the Arduino project. Now that I had a better understanding of what inputs I was looking to build, I drew up a basic diagram for the panel. After some iterations, it looked like this:

![Panel Design](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/panel-design.png)

I had a rough idea of what each button would be/look like, but no idea of what it would actually be, or where I would find it. 

## What is a Switch?
The internet is full of buttons and switches, of all shapes and sizes. Google searches led me to [Mouser.com](https://www.mouser.com/), a button store. I hopped on the site, navigated to the mechanical switch section, and found a filter for "toggle switches". I was then presented with 140k products! And thus began the great mechanical switch research project of 2019, an adventure I shall not bore you with.

## Test Switch
Now that I had a better idea of what kinds of switches were out there, I ran to a local electronics store and bought a few to test with.

I bought a mini toggle-switch single pole, single throw with a (ON) OFF (ON) configuration. The parentheses around the "ON" states means that it's a momentary action. This is important because within a button matrix, leakage on closed circuits can be a [problem](https://www.baldengineer.com/arduino-keyboard-matrix-tutorial.html). Since I was effectively simulating a standard computer keyboard -all of those keys are momentary buttons- and I wanted to reduce the electrical wiring complexity, I opted for momentary switches across the board (mostly). So back to the switch I bought, it can be bumped up or down, and this can be treated as separate key presses. 

I was then able to wire together an example circuit with the switches I bought using a breadboard. The switches were arranged in a matrix formation. This is where the [Keypad](https://playground.arduino.cc/Code/Keypad/) library saved the day. A keypad object is created with the number of rows and columns in the matrix. The library then handles the logistics of sending signals through each row and reading each column. It even uses the Arduino internal pull-down resistor to negate phantom signals and keep readings accurate. 

I found my way to [this](https://github.com/Anarch157a/Elite-Dangerous-Keypad) great example of using it, which I slowly read through and broke down into understandable parts. This allowed me to understand what pieces I needed, what abstractions I could draw, and how much of the details I could punt on until later vs how many I needed to nail down before buying the rest of the buttons. 

## Over Complicated Hyperspace Jump Design 
I wrote up some skeleton software for the Arduino and investigated the silly ideas I had about making the jump to light speed system waaay over complicated.

I devised a [three stage system](https://github.com/westonnovelli/project-actuator/blob/master/src/motivator.ino) for making a hyperjump. My hyperdrive consisted of two field vector motivators that assisted in powering the drive. In order for the motivators to fire, they needed to be primed before every jump. Further, each motivator was configurable to two vectors. These vectors would have to be alternated for each jump. Think of an hourglass, once it's drained to the bottom. You have to flip it to use it again. Using the 2D toggle switches, I would prime each motivator into either the positive or negative vector. Only after both motivators were primed in the opposite direction than it was previously primed (and fired) would the jump drive be triggerable. Then I had a mode switch that would allow me to make either a supercruise (intrasystem travel), hyperspace (intrersystem travel), or a drive disengage shift. All of this complexity to make it more fun and exciting when I needed to jump in a hurry. I can pull the whole "[watch this](https://youtu.be/X-rkFaIPyL4)" line from Empire Strikes Back. 

All of this logic would be handled by the Arduino software. The result to the computer would be 1 of 3 different keystrokes depending on the mode switch. The motivator logic and mode detection was taken into consideration when the jump button was pressed.

This function is called when the jump button is pressed. 
```c
bool handleJump(char key) {
  if (key == 'l') {
    if (allPrimed()) { // we have successfully primed all motivators
      if (jumpMode == 1) {
        queueModifiers(_SUPERCRUISE_MOD);
        queueKeystroke(_SUPERCRUISE);
      } else if (jumpMode == 2) {
        queueModifiers(_HYPERSPACE_MOD);
        queueKeystroke(_HYPERSPACE);
      } else {
        queueKeystroke(_JUMP_DISENGAGE);
      }
    } else {
      // Failed jump attempt; reset motivator for next discharge
      resetAllMotivators();
    }
    stabilizeAllMotivators();
    return true;
  }
  return false;
}
```

The `allPrimed` function checks to see if every field motivator passes the primed test:
```c
/*
 * A Motivator is only primed successfully if the newly set vector is NOT identical to the previously set vector.
 */
bool primed(int index) {
  return motivators[index][prev] != motivators[index][current];
}
```

The Keypad logic is piped through a `handleKeypress` function that switches on the virutal key that was pressed, i.e. `handleKeypress` determines which button was pressed.

```c
bool handleKeypress(char key) {
  if (key == 'A' || key == 'C') {
    return handleMotivator1(key);
  } else if (key == 'B' || key == 'D') {
    return handleMotivator2(key);
  } else if (key == 'l') {
    return handleJump(key);
  } else if (key == 'P' || key == 'O') {
    return handle0Throttle(key);
  } else if (key == 'M' || key == 'N') {
    return handle50Throttle(key);
  } else if (key == 'd' || key == 'c') {
    return handle75Throttle(key);
  } else if (key == 'a' || key == 'b') {
    return handle100Throttle(key);
  } else if (key == 'k' || key == 'j' || key == 'i') {
    return handleSafety(key);
  } else if (key == 'h' || key == 'g' || key == 'e' || key == 'f') {
    return handleShipPeripherals(key);
  } else if (key == 'E' || key == 'F' || key == 'G' || key == 'H') {
    return handleVisualAugmentation(key);
  } else if (key == 'I' || key == 'J' || key == 'K' || key == 'L') {
    return handleTravelAssist(key);
  }
  return false;
}
```

In an attempt to organzie the logic and separate concerns, handler functions were written to abstract button groups.
Example handler function:
```c

bool handleTravelAssist(char key) {
  if (key == 'I') {
    queueModifiers(_OPEN_GALAXY_MAP_MOD);
    queueKeystroke(_OPEN_GALAXY_MAP);
    return true;
  } else if (key == 'J') {
    queueModifiers(_OPEN_SYSTEM_MAP_MOD);
    queueKeystroke(_OPEN_SYSTEM_MAP);
    return true;
  } else if (key == 'K') {
    queueModifiers(_NEXT_SYSTEM_IN_ROUTE_MOD);
    queueKeystroke(_NEXT_SYSTEM_IN_ROUTE);
    return true;
  } else if (key == 'L') {
    queueKeystroke(_DISMISS_RECALL_SHIP);
    return true;
  }
  return false;
}
```
The return value says whether a key was queued or not.

Lastly, the `config.ino` [link](https://github.com/westonnovelli/project-actuator/blob/master/src/config.ino) was used to separate the actual keybindings from the logic. Using `#define` to create constants, the keybindings can easily be switched without worrying about the logic.
```c
#define _SUPERCRUISE_MOD KEY_MOD_LCTRL
#define _SUPERCRUISE KEY_J
``` 

## Purchasing All the Buttons
Now that I had some fun writing software it was back to button research. My trip to the local store gave me a better idea of what these online products were like and so I dove in... slowly. After a few more weeks of research on buttons and switches, I had picked out all my actuators of choice and placed the order. The buttons were delivered the next day! I quickly wired each type of button into my prototype circuit to confirm it worked in the matrix configuration as I had planned, and they did! Yay! The next step was to figure out how to mount all of these buttons and switches.

## Side Story about Safety Switch 
Well, except for 1 hiccup. Surprisingly, I could only find one toggle switch safety cover on the site. The switch configuration I needed was not produced by the same manufacturer as the cover so I did my best to compare measurements and find a compatible option. I was able to... almost. The safety cover doesn't allow the switch to be flipped when closed. When you open the cover, the switch can then be flipped, but now it's difficult to "unflip" the switch. When you close the cover it will unflip the switch for you. Being able to manually unflip the switch wasn't a problem for me because the switches were a OFF (ON) configuration so they bumped back to OFF by themselves. The problem was the direction they rested in, it was backward from the safety cover. The cover and the switch had a notch that would keep the cover from rotating around the neck of the switch, but the switch rested in the "unsafe" position. Luckily, it wasn't too difficult (or hazardous) to disassemble the switch housing, flip the neck component, and reassemble. Disaster Averted!

## Panel Construction 
After some deliberation, I decided to use some sheet metal as the panel material. Using a thin panel would have its benefits, but metal is largely unfamiliar to me. I've been a hobbiest woodworker for a few years and have a good understanding of lumber, but metal was going to be a new adventure!

I decided (without much research) that the 22mil plain steel sheet would be the right strength for my needs. And thus, the construction (and learning) process began.

Using sheet metal snips, I cut off a small test piece to practice with. Using standard drill bits I slowly and carefully drilled holes of varying sizes for the different buttons. The dimensions worked out rather well and matched the drill bits nicely. I found that using smaller bits to start, and gradually increasing the size resulted in the cleanest hole. The next obstacle was the square holes I needed. After some testing with a dremel, I found using a triangle file resulted in the best and tightest corners. Unfortunately, I didn't have a file small enough for the smallest squares and the was rather slow going. So a quick trip to the hardware store, a new small triangle file and a tungsten carbide cutter attachment for the dremel later, I was back in business. I also found a set of countersink bits that allowed for even cleaner, and larger holes to be drilled. The dremel would hog out the most of the corners and the file would square everything up.

![Practicing Cutting the Holes](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/test-piece-cuts.jpg)

I fit each button type into its test hole and confirmed everything fit. Next step, cut it all for real!

![Testfitting the buttons in the test piece](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/test-piece-fitting.jpg)

I cut the panel to size, marked each hole, and drilled initial small holes for each button.

![Pilot holes for Layout](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/initial-holes.jpg)

Slow and careful work expanded each hole to the exact dimensions. One hole needed to be a hexagon, the dremel and file technique worked perfectly.

![Cut holes to Size](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/shaped-holes.jpg)

Now I was able to mount each button into the panel, next up: wiring! 

![Test Fit of All Actuators](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/test-fit.jpg)

But before I wired it all together. I decided to engrave some symbols into the panel to spice up the design and, perhaps, make each button more identifiable. I wanted to be able to use this panel for more that just Elite Dangerous. It was, after all, just emulating a keyboard. So, I didn't want to make labels that were space ship/Elite specific. So I decide to put largely random Aurebesh characters on it, because I like Star Wars. I even designed and drew a Star Wars inspired symbol on it (bonus points if you can figure out what SW symbols I worked from).

![Aurebesh Markings](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/markings.jpg)

## Wiring
I headed back to my drawing and virtually wired (and then rewired) my matrix. In the end, I had this:

![wiring-diagram.png](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/wiring-diagram.png)

I color coded the columns vs the rows and numbered each one with the pin it would connect to on the Arduino. Then I began wiring. Yikes, this was a bit of a mess. To make a long story short (too late), DON'T USE PLUMBING SOLDER FOR WIRING. Even if it won't corrode the electronics and even if the connections are good enough to conduct, it's not worth the hassle. Once I bought some proper electrical solder and stopped using the old (apparently plumbing) solder I had lying around, it was actually quite clean and nice to work with. A touch of rewiring later, I was all wired up... with the matrix at least.

![Wiring](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/wiring.jpg)

## Second Circuit
Most of the buttons on the panel were a part of the matrix, but a few didn't belong. The mode switch for the hyperspace jump mechanic was a rotary switch and thus not a momentary actuator and the full panel ON OFF switch were handled separately. This is where I finally understood what pull-down resistors are for!

A small secondary circuit was added. I wanted to be able to disconnect the panel from the computer without having to unplug it. Further, I only ever needed to read the rotary switch's state when the jump button was pressed. These could easily be wired together, or so I thought. My first pass resulted in some very confusing results and it turns out, the phantom signal problem could be solved with a pull-down resistor. And so the circuit was reconfigured to look like this: note the resistor!

![Pull-down resistor circuit](https://www.arduino.cc/en/uploads/Tutorial/button_schem.png)

The panel ON OFF switch logic was inserted into the serial writing function such that before the Arduino writes bytes to the serial port, it checks to see if the panel active switch in ON or OFF.

```c
  // loop() function
  ...
  // update active state based on on/off switch
  panelActive = digitalRead(signalIn) == HIGH;

  ...
  // function called to send the keystroke to the computer.
  void serialWrite(uint8_t buf[8], int len) {
    if (panelActive) {
      Serial.write(buf, len);
    }
  }

```

## Final Software Touches
And then all was mechanically complete! I finalized the rest of the software, referenced my key-binding notes to complete the keystroke configuration, and tested that pressing a button made those keystrokes happen! I used one of those presenter assistant programs that displays what keys were just pressed in the corner of the screen. [Carnac](http://code52.org/carnac/) worked just how I needed to see the modifier keys (CTRL, shift, ALT, etc) and everything.

![Full Installation](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/installation.jpg)

## Housing and Mounting
The final phase was near: build the panel housing and mounting system.

The housing was cut from some scrap quarter inch plywood I had. The backing panel was the leftover sheet metal. Nothing was quite cut square, so it's a bit janky, but it still works.

![Full Setup](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/full-setup.jpg)

All is said and done, the project took around 5 months of on and off casual work.

![Elite Dangerous Usage](https://raw.githubusercontent.com/westonnovelli/project-actuator/master/images/panel-usage.png)

Thanks for reading! I welcome feedback and thoughts.

o7 CMDRs

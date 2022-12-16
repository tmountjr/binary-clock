# binary-clock
24h clock showing binary numbers rather than decimal numbers.

Pretty simple concept: in a 24-hour clock, the hours. minutes, and seconds need no more than 6 binary bits each to display the max number for that slice of time. So why not display those bits visually?

As an added visual bonus, as each slice progresses from its min to its max, it will make a complete circle around the color wheel.

Future work will include a web interface to set the timezone offset and...other things that you might find in a web interface.

Hardware:
* 18x WS2812B 5050 LEDs (I just removed them from a strip I had laying around)
* 1x 220 ohm resistor
* 1x NodeMCU (too lazy to implement discrete components)
* 2x headers

Video of the clock in action: https://photos.app.goo.gl/k7p5UYowemyLLavA8

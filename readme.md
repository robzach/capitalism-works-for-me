My work log for repairing Steve Lambert's [*Capitalism Works for Me*](https://visitsteve.com/made/capitalism-works-for-me-truefalse/) piece for inclusion in the [Marx@200 show](http://www.spacepittsburgh.org/portfolio-view/marx200/) organized by CMU Profs. Kathy Newman and Susanne Slavick. Short-term patching repair work carried out in April 2018, installation mounted May 1–16, and follow-up long-term repair completed in June.


# repair log and notes

## 4/2/18 noon–4:15 p.m.: arranging

Kathy and I moved the sign pieces around to begin to arrange them for repair in her garage. It was approximately a four-person job! Kathy stood in for three and I was myself.

## 4/4/18 1:45–4:30 p.m.: assembly, rearranging, electrical debugging

* I helped Kathy assemble a space heater to make the garage a bit more usable in the unusually chilly April weather.
* I did a bit of electrical debugging, noting that the Arduino ground on the sign is not in electrical contact with the metal case (i.e. AC power) ground. I didn't connect the two, in the event that there was a ground loop or other issue that would make connecting them screw something up. But it's worth noting.
* Kathy and I set the signs on newly-bought folding tables, so they're much easier to work on.

## 4/5/18 an hour: connector looking

I wanted to buy some connectors so that I could wire the second half of the lower part of the sign to the first without them being right next to each other (our garage workshop can't accommodate them laid out end-to-end). I don't have a BOM or other technical documentation from the original fabrication, and couldn't figure out what the connectors were. I made [a post](https://electronics.stackexchange.com/questions/366733/help-identifying-connectors-from-an-art-project) in Electronics Stack Overflow to solicit community help, and got good answers. (They are likely [this class of Molex](https://www.molex.com/molex/products/datasheet.jsp?part=active/0003091042_CRIMP_HOUSINGS.xml).)

Subsequently, I realized I could just buy my own new connector to carry all 8 wires at once, instead of replacing these old connectors or making my own extension cable for temporary use. I placed an order for [this model](https://www.amazon.com/gp/product/B01CRV0RUM) 12-conductor waterproof connector on Amazon, which is scheduled for delivery Sunday or Monday. I'll install a longer wire (like 10' or so) so that I can light both halves of the sign while it's in the garage space.

## 4/6/18 1:15–5:15 p.m.: communication debugging, documentation, a bit more ordering

I suspect communication trouble over the current ethernet cable that's set up.

When running `f7f19e3` (initial commit) of `capPodiumV3_rz_debug_edit.ino` (on the podium) and `capSignV16_rz_debug_edit.ino` (on the TRUE side of the sign), I was trying to debug the wired communication over the RS485 lines. 

Watching the serial monitor on the sign side, I could not receive signals sent by the podium when I pushed the button. (I knew the podium was trying to transmit these signals from its own serial feedback.) While wiggling the female–female ethernet connectors around, this sequence of garbage data printed:

<pre>
data received from podium: 0
data received from podium: 9
data received from podium: 140
data received from podium: 0
data received from podium: 4
data received from podium: 64
data received from podium: 144
data received from podium: 76
data received from podium: 128
data received from podium: 50
data received from podium: 160
data received from podium: 144
data received from podium: 153
data received from podium: 201
data received from podium: 0
data received from podium: 153
data received from podium: 50
data received from podium: 144
data received from podium: 0
data received from podium: 136
data received from podium: 128
data received from podium: 232
data received from podium: 0
data received from podium: 144
data received from podium: 16
data received from podium: 16
data received from podium: 0
data received from podium: 0
data received from podium: 0
</pre>

So: looks like the RS485 isn't so reliable, or at least it's easy to make up garbage data by moving it around. I suspect the connectors aren't so great, which may be part of why the signal isn't making it from the podium to the sign.

A test to run: move the podium close to the sign so that the connection is just:

<pre>
podium     short wire      female–female jack       short wire       sign
]---------------------------------====-------------------------------------[
</pre>

An unnecessary drawing to show how I can test one jack at a time, instead of what I had before:

<pre>
podium     short wire      female-female jack    long coiled wire      female–female jack     short wire    sign
]--------------------------------====-----------------------------------------====-------------------------------[
</pre>

Result of that test: confusing conclusion.

* One connector, which I've since labeled "iffy!", produces lots of `data received from podium: 0` lines when wiggled.
* The other connector, which I've labeled "good?", produces no output at all when wiggled. That seems much better to me.
* But *neither of them* shows podium data getting to the sign. (I.e. pushing "true" or "false" on the podium, which you can tell is working somewhat because the 15 second counter resets, does not produce any serial feedback on the sign side.)

Obviously, these wires shouldn't expect to be actively jostled while the piece is installed. But the fact that the data isn't getting through, and that one of the connectors looks wonky, makes me think it could be part of the problem. I'm going to order a new connector to be safe. Picked up a [3-pack for $8](https://www.amazon.com/gp/product/B06ZYN4RX6) on Amazon, delivery is due Monday.

### software oddities

#### compilation failure

I needed to make some small changes to the software as I'd received it so that it would successfully compile in Arduino 1.8.5, which is the version I'm running on my computer. I'm noting them here explicitly for posterity:

original (capSignV16.ino) | changed to (capSignV16_compile_Arduino185.ino)
--- | ---
`Wire.send()` | `Wire.write()`
`Wire.receive()` | `Wire.read()`
`sevenSegDisplayString(count, true)` | `sevenSegDisplayString((String)count, true)`
`sevenSegDisplayString(count, false)` | `sevenSegDisplayString((String)count, false)`

The `Wire.send()` and `Wire.receive()` commands apparently changed nomenclature, which the compiler told me about. The other problem, which I solved by casting the `int` `count` to a `String`, I didn't get a clear warning about.

#### some sort of fencepost issue?

Uploading `capSignV16_compile_Arduino185` (commit `94f944`) onto the sign module, I saw an old familiar number show up in the serial feedback:

<pre>
Total vote count true eeprom : 4294967295
Total vote count false eeprom: 4294967295
Last vote count true eeprom: 4294967295
Last vote count false eeprom: 4294967295
Write long eeprom
Write long eeprom
Current TRUE : -1
</pre>

Those eeprom values are the maximum `unsigned long` the Arduino can hold. For some reason, the TRUE count is going to –1, and those eeprom values are going to their maximum. I haven't tried solving this yet, but it is a low-priority problem in any case.

## 4/12/18 2–6 p.m.: communication troubles persist; slimming the podium-side code slightly

### ethernet connector testing

I substituted in a newly arrived ethernet connector for the podium-to-sign signal test. It didn't fix the problem. Wiggling this new connector didn't cause spontaneous random signals to be transmitted, as wiggling the old one had, but unplugging and replugging the cables does transmit `0` signals. In any case, the votes are not being transmitted from the podium to the sign.

I wrote and uploaded diagnostic sketches, which appear in commit `9cc4e11`, and it appears that communication over Serial2 is not making it from the podium to the sign. (The podium says it's sending the data; the sign doesn't receive it; making and breaking the CAT5 connection makes it look like it is at least listening to those pins properly since garbage appears in the serial monitor when there are electrical signals.)

Started doing some electrical investigation. I found that on the podium RS485 module, the Arduino ground was plugged into the module's TX, and the Arduino's serial signal wire was floating in the air, connected to nothing…but close to the board and bent as if it had been plugged in. I don't remember unplugging any wire and replugging it (partly because I would've had to have looked up the wire mapping, and am pretty sure I didn't do that), but at least this is an electrical problem I can try to fix.

Somehow fixing it didn't matter! And there's something odd going on electrically on the Mega: its own 3.3V pin has a consistent 4.69V with respect to its own ground. I don't know that that's a definite problem, but it's unusual and points to potential trouble.

After uploading the diagnostic sketches, I noticed the RTS line on the transmitting side (the podium) wasn't being used in my own sketch. This line needs to be held high on the transmitting side of the CAT5 link, so I added lines to pull it high at all times (commit `55f9df1`). Didn't help, but now when the connection is made and broken repeatedly, instead of garbage data, the sign Arduino just shows `0`s on the Serial2 read. Interesting.

Then I pulled the RTS line low on the receiving side (commit `45ab34c`), and it continued to not work.

As a test, I wired the TX2 pin of the podium Arduino directly to the RX2 pin of the sign Arduino, and tied their grounds together as well; this successfully transmitted `1`s and `2`s as it should've. It's obvious that the problem is not the fundamental serial stuff in software, but rather the transmission via RJ45. Further work here is needed.

### trouble compiling the podium-side code

The podium Arduino relies on a family of libraries called RF22, which have since been superseded by the RadioHead library by the same author. I wasn't able to compile the podium code (capPodiumV3_rz_debug_edit.ino) as it was missing all those library files locally. They're still available online and so I added each necessary header directly to the sketch folder (this attempt appears in the manualAddHeaders branch, commit `2cc55796`), but after resolving missing files, still had some bad-looking low-level library problems. To wit:

<pre>
ccvts0YB.ltrans0.o:(.text.startup+0xfa): undefined reference to `RF22ReliableDatagram::RF22ReliableDatagram(unsigned char, unsigned char, unsigned char)'
/var/folders/86/fxc5555514l737wd7rb8dt6w0000gn/T//ccvts0YB.ltrans0.ltrans.o: In function `main':
ccvts0YB.ltrans0.o:(.text.startup+0x208): undefined reference to `RF22Datagram::init()'
collect2: error: ld returned 1 exit status
</pre>

Seeing that these problems were not the sort I wanted to work on, I tried instead to use the RadioHead library, which the author says is backwards compatible with RF22. Apparently, for me, that's not the case.

Because it appears that the RF22 code all supported using radio transmissions of data which are no longer used in the project, I'm planning to cut all of it out and avoid the problem altogether. This appears in commit `29d1446`, and it compiles without complaint.

### wires to be worked on next

I have the waterproof connectors as well as a crimping tool to connect them, but need to buy longer wires to complete the job and allow the left side of the sign to connect at a distance to the right side via the new connectors. I will buy the wires tomorrow morning and install them.

## 4/13/18 11:30 a.m.–1:30 p.m.

Replacing the electrical connectors between the bottom halves of the sign.

Wire numbering system for my own reference:

number | color
--- | ---
1|orange
2|yellow
3|blue
4|brown
5|black
6|green
7|light brown
8|white
7b|light purple (common with pin 7)
8b|grey (common with pin 8)

## 4/13/18 2:00–5:45 p.m.

### digits

After connecting all wires except for the pair that runs the large LED illumination of "FALSE", the "false" side of the sign is in good working order, with all 7-segment digits working properly. Good!

On the TRUE side, I had to replace one bad LED segment, and in another case resoldered one with an iffy connection. Following that, I get a solid 888 will all segments appearing. Also good!


### communication

Instead of using the RS485 protocol, I tested simply running a long pair of wires from the podium to the sign: GND to GND, and the podium's Serial2 transmit pin (pin 16) to the sign's Serial2 receive pin (pin 17). The wire I'm using is shielded and I used the shield conductor as the ground to reduce noise. It appears that data is transmitting reliably based on some testing, and so I'll plan to simply discard the RS485 altogether and use my new *very* simple interconnect for communication between the podium and sign.


### power wiring

The only wiring that's incomplete between the TRUE and FALSE sides is the power to run the illumnation LEDs behind the always-lit word FALSE. The wire that was originally used for this connection is 18AWG, but the individual wires in the multiconductor cable that I'm using to connect the two sides is probably more like 20 or 22 AWG (I'm not sure). Because I don't want to have any problem with undercurrent (or overheating), I'm ganging conductors 8 and 8b together, and 7 and 7b; I think two of the smaller wires in parallel will suffice. This last little bit of wiring is about halfway done as I'm finishing work for the day and should only take less than a half hour to complete.

## 4/25/18 8–9:30 p.m.

Rewrote EEPROM code on the podium side. This work appears in the `eepromModification` branch.

## 4/26/18 9:30 a.m.–12:30 p.m.

Completed the power wiring so that the FALSE side has backlighting (and digits are still working).

Further modified the podium side EEPROM code so that it actually remembers last values after a power cycle; at the moment it increments both TRUE and FALSE counts by one on restart, but this doesn't seem like an important enough bug to care about squashing.

## 4/26/18 2:45–4:15 p.m.

Completed chassis ground point installation: the TRUE and FALSE sides have grounding pigtails which are tied to each other through the data connection cable ground wire. Based on current progress, compiled a shopping list of items to pick up at Home Depot or bring from home:

* ground bus bar (2)
* wire nut to join for 1 12AWG and 1 18 AWG wire (12)
* grounding pigtail (3)
* male plug with ground wire (1)
* stranded black or green wire, low voltage, ~20AWG (20 ft.)
* self-tapping sheet metal screws

## 4/29/18 2:30–6:45 p.m.

Added proper grounding wiring to the TRUE sign. Completed connections using new connectors (i.e. podium has a leash which leads to TRUE side, and FALSE side has a leash which leads to TRUE side as well; both have marked plugs inside TRUE cabinet).

Updated version to v.17 to reflect working software, tagged in git (commit `732e881`). N.b.: there is more software work that *could* be done, but it's not truly necessary. (The biggest bug is that the votes both increment by one when the sign is power cycled…not terrible.)

Updated main to-do list with progress.

Finished up work on the TRUE and FALSE signs, and put them back in their crates for transportation to campus—however, as we were loading the FALSE side, we noticed that the seven-segment display was only attached to the sign on one side and was flexing and hinging around the other side. Not so great! This will have to be mechanically repaired in the assembly and mounting process.

Deleted extra files used during development to clean up the repo. (Go through the history to find these if needed/wanted.)

## 4/30/18 11:30 a.m.–6:30 p.m.

Moved crated signs from Kathy's house to campus, and assembled the sign in front of CFA.

## 8–9 p.m.

Interconnected two lower sign sections via cables. Ran power to podium, and from there to the sign as well. Started up sign and after resetting EEPROM it ran properly for a half hour or so, and then shut down. To be investigated 5/1.

## 5/1/18 1:15–4:00 p.m.

Began debugging the sign's apparent shutdown. Plugged it in and it worked anew. Then, while adjusting cables to tape them securely to the ground, the suddenly turned off. Power cycled again, and it resumed working. There is something iffy going on here. Some 7-segment segments may have burned out; this isn't clear without setting to 888 and since people were actively using the sign, I left that unmodified, to recheck later.

Added new connector to marquee lighting so that no Wago connectors or wire nuts are needed for breaking and making that connection. Will do the same in replacing the other marquee connector, which was somewhat damaged in transportation (I temporarily made one of its connections with a wire nut).

Moved the remaining crate to Kathy's office for temporary storage.

## 5/14/18 noon–1 p.m. and 3–6:30 p.m.

Work on assembling new (version 18) software and hardware.

## 5/15/18 3–5:45 p.m.

Work on assembling, testing, and correcting new electronics hardware.

## 5/16/18 9 a.m.–1:30 p.m., 2–5 p.m., and 9:30–10 p.m.

Work on new electronics (9–10:30 a.m.); disassembly of the installation (10:30 a.m.–1:30 p.m.); further disassembly and cleanup, including covering crates with tarps for overnight rain protection (2–5 p.m.); and preparing overnight stress test of new electronics hardware and software (9:30–10 p.m.).

## 5/17/18 10 a.m.–12:30 p.m.

Move CAPIT, ALISM, and sign bracket crates to FMS warehouse for temporary storage.

## 5/21/18 6:30–8 p.m.

Compiling work log and accounting for all purchases.

## Hours worked up to this point: 63 hours

## 5/24/18 5:45–10 p.m., 10:45–11:30 p.m.

Opened up back of TRUE side. Tested driving TRUE side seven-segment display ([manufacturer's documentation of the Big Digit Display](https://wiki.evilmadscientist.com/Using_The_Big_Digit_Driver#Communication)) with my new hardware using the SPI interface, and it is successful (digits display 456 upon running the test code, `bigDigitDisplaySPItest`, commit `1af5522`). 

The 2&times;3 male header on the Big Digit Display needs to have four connections made as per the standard SPI scheme:

connect to | 2&times;3 pin # | 2&times;3 pin # | connect to
--- | --- | --- | ---
Arduino MISO | 1 | 2 | not used
Arduino SCK | 3 | 4  | Arduino MOSI
not used | 5 | 6 | ground

Pin 1 on the Big Digit Display is marked with a triangle on the PCB.

Rewrote `capDigitDisplay` sketch to include both SPI driving code and serial (RS485) data in. After trying to incorporate the newer SPI library (which postdates the original documentation for the display), I reverted back to pushing out data in the simplest, byte-by-byte fashion, in the mold of the example code supplied by the manufacturer. The SPI library simply wasn't updating the display despite some long attempts.

`capDigitDisplay`, commit `83e7f96` on `v18dev` branch, successfully receives RS485 data and prints the last three digits of any transmitted number to the 7-segment display it's attached to via SPI.

`podium7segdisplay`, commit `29bcd67` on `v18dev` branch, successfully displays a countdown from 15 seconds on the podium's 7-segment display via SPI. It will need to be expanded to:

* read the two voting buttons,
* send vote totals via RS485 once per second,
* store vote totals to EEPROM in case of power cycling (and read last stored values at startup), and
* read case-mounted pushbuttons for manual vote resetting, 888 digit testing, etc.

## 5/25/18 a half hour

Emailing with Windell Oskay of Evil Mad Scientist, vendor of the Big Digit Driver, about technical troubleshooting. Thanks for your help and advice, Windell!

## 5/26/18 10–11 a.m., 1–5:15 p.m.

Using new ribbon cable and 2&times;3 DIN female socket to connect digit-driving Arduino to the Big Digit Driver; works fine upon testing.

### Attempted overcurrent debugging on 7-segment display, TRUE side

I'm using an ammeter in series with Big Digit Display's 36V supply line to test current on each segment; if it's too high, that could be the culprit for segments burning out. As per Windell Oskay's advice, each segment should be drawing 78mA.

The quiescent (no segments lit) draw of the driver circuitry is at the edge of my multimeter's lower range: it's between 0.00A and 0.01A, so we'll call it 5mA.

I wrote `serialCommandsToLightBigDigitSegments` to allow for testing of each individual segment and investigate for overcurrent, which may be burning segments out. Runnning commit `fc0aab6` on `v18dev` branch. 

**Test 1:** Current draw on individual segments, lit one at a time. Note that segment designations are from the [manufacturer's documentation of the Big Digit Display](https://wiki.evilmadscientist.com/Using_The_Big_Digit_Driver#Communication).

digit | segment | current draw
--- | --- | ---
right | A | .08A
right | B | .09A
right | &#8942; | .09A
right | G | .09A
center | A | .09A
center | B | .09A
center | C | .09A
center | D | .09A
center | E | .01A (this segment is burned out)
center | F | .09A
center | G | .09A
left | A | .09A
left | &#8942; | .09A
left | G | .09A

In conclusion, everything's drawing just about the same, except for the burned out segment. And everything's drawing a hair too much, but steady state overcurrent does not appear to be the problem.

**Test 2:** Current draw on multiple segments lit concurrently, to make sure everything's adding linearly as it should be.

digit | segments | current draw | avg. current/segment (calculated)
--- | --- | --- | ---
right | A,B | .17A | .085A
right | A,C | .17A | .085A
right | A,B,C | .25A | .083A
right | A,B,C,D | .33A |.083A
right | A,B,C,D,E | .41A | .082A
right | A,B,C,D,E,F | .49A | .082A
right | A,B,C,D,E,F,G | .58A | .083A

Looks fine. Again, current per segment is slighly higher than would be desired—83mA instead of 78mA—but that small difference is unlikely to be the culprit.

**Conclusion from tests 1 and 2: overcurrent isn't the likely cause. Look for grounding trouble.**

Putting off that question for the moment, worked to put the pieces together, read the podium buttons, and send data over the cat5 cable to the receiving board in the sign. It works!

Sketches that are working: on the podium controller, `capPodiumV18`; on the digit driver, `capDigitDriverV18`, both commit `232cba6` on `v18dev` branch.

Ordered more materials needed to complete fabrication: VHB tape, grounded plug, screw terminals, pass-through ethernet extension cables, silicone sealant, and dessicant packs.

## 5/29/18 7:15–11:15 p.m.

Investigating the ground fault, I scoped the difference between the Arduino's 12V DC supply with respect to the chassis of the sign; it had a huge swing with respect to it, in part because the big chassis itself catches some stray EMF. I will definitely need to tie the logic ground to the chassis to get rid of that big voltage between them.

I mistakenly didn't add ground or 5V points in my PCB design (oops! Was maximizing available GPIO points) but this is easily enough solved by soldering jumpers onto the ground and 5V of the 7805 regulator.

Made a new grounding plug for the podium (so that its chassis can be tied to electrical ground for safety).

Wired up a logic ground bus on the TRUE side (this is the side of the sign I've been working on so far), so that the Arduino's logic ground can be reliably tied to the chassis. After doing this, but before turning on the breaker to allow line power to flow to the 120V electrical systems on the sign, I scoped the SPI logic signals from the Arduino to the Big Digit Driver, and they looked like reasonably good logic signals, switching between 0 and 5V without too much noise.

HOWEVER: there is significant noise between the Arduino ground and the chassis (prior to connecting them electrically). The scope's ground is on the chassis, and its probe on the Arduino ground:

<img src="/images/59KHz_hum.bmp"/>

(Captured at the surprisingly low display resolution of the scope I'm using.) The dominant bumps have a period of ~17µs, so a frequency of ~59 KHz. This segment of the signal looks like it has a +4V DC offset, but it's actually just the top part of this 60 Hz wave:

<img src="/images/60Hz_hum.bmp"/>

Also worth noting: when the line power is switched on, so that the seven-segment LEDs are illuminated, if I make contact between the Arduino ground and the chassis, the LEDs which are off get a rapid, light flicker to them. Not good.

IN UNRELATED NEWS: when I plugged in the podium power, instead of showing `00:15` and counting down, it's showing weird garbage like `0  0` or `FF0 ` or `0 00`. Not especially reassuring, since this was working without any trouble when I last used it. Tried debugging this, writing and uploading an SPI 7-segment-writing sketch, which worked without any trouble (that sketch: `SPI_7-seg_countup_test_for_podium_debugging`, commit `6ed6093` on `v18dev` branch). Since I'm not aware of anything having changed—electrically or otherwise—I don't know now why the podium's 7-segment display would now be acting up. When it rains.

Calling it a night since many things are going wrong and I don't want to cause any more problems!

## 5/30/18 3–6 p.m.

The one and only Garth Z. helped me with debugging this afternoon. The flicker in the Big Digit Display LEDs appears, fingerscrossedly, to be somewhat aided by a small change in the code driving it: slowing the SPI data rate makes something of a difference. (It still flickers a bit, but it's improved.) The relevant changes appear in `capDigitDriverV18`, commit `492e788`, `v18dev` branch.

Trying to track down the flickering, we were, among other things, scoping the +36V supply for the BDD, and found that simply touching the oscilloscope's probe to the 36V supply terminal on the back of the ones digit would induce a big flicker across all the digits. Similarly, we observed that plugging and unplugging a small DC transformer (one of the 12V Arduino supplies) *simply on the same circuit* as the rest of the cabinet would reliably induce a big flicker. The tentative conclusion is that the BDDs are very sensitive to very small, transient changes in the supply voltage. These changes were too small and/or brief for us to be able to actually scope them.

A simple solution, which I'll try tomorrow, is putting a 50V capacitor across the 36V terminals behind the ones digit: if that helps smooth the apparently harmful transients, then that's certainly an easy solution.

Perhaps if small supply voltage transients can cause flickering, which may mean brief low-current conditions, then it is also capable of causing brief *high-current* conditions as well. If so, that could help explain the segments burning out. Absolutely unclear, but it's a possibility.

Finally, we mapped out the logic ground for the whole project, which is not the same as the chassis (Earth) ground. In fact, at the 36V supply, the V- terminal floats about 200mV above the chassis. Moving forward, I will isolate the logic ground from the chassis; I originally thought tying them together would help solve problems, but I now believe it's now more trouble than trouble-solving.

## 5/31/18 an hour

Used a bench supply set at 36V to try to determine the effect of a lower-noise power supply on the flickering of the LEDs. It did appear that there was less flickering. However, scoping the supply point of entry to the still showed more than a volt of peak-to-peak noise; this noise, apparently, is getting introduced along the path somewhere (which is confusing because it's a twisted pair that travels about 8 total feet through a metal pipe).

On Garth's advice I also did some poking around looking at voltages on the BDD board; I saw correct (10V) falls across both the Zener diodes, perfectly fine (5.08V) supply at the VCC of both logic chips, and measured that all the 16Ω current sense resistors were, in fact, 16Ω within a few percent. I also removed the BDD's 74595 and manually jumpered 5V to each of the output pins, and cycled through all of the 7-segment segments without any problem.

More figuring to do.

## 6/1/18 2:15–3:45 p.m.; 4:30–5pm

Placed a 50V, 100µF electrolytic cap across the power supply port of the BDD and saw a huge smoothing effect.

Without the cap (running off of a bench supply):

<img src="/images/supplyAtBDD.jpg"/>

With the cap:

<img src="/images/supplyAtBDDwithCap.jpg"/>

(Didn't have a USB stick on me at the moment so these are cell phone photos.)

Despite smoothing the power significantly, there is still a flicker whenever the power in the sign is changed at all; if anything is plugged in or unplugged, the LEDs flicker.

### The grounds aren't isolated from each other?

The chassis ground should be totally isolated from the logic ground at this point, but I am able to consistently measure a 30Ω resistance between the two, so I'm now going to look for any sort of leak which may be the underlying problem. Additionally, I measure that with respect to the logic ground, the chassis ground is 3.5mV higher in potential. If these two things are consistently true, *V=IR* says that a stray 0.1mA should be constantly flowing from the chassis ground to the logic ground. I don't think a tenth of a milliamp is the problem here; I think the 30Ω resistance between the two grounds—rather than hundreds of thousands of ohms or greater—is the problem. They don't appear to be isolated.

### The multimeter's asymmetric behavior

The multimeter reads a steady 3.5mV difference between logic ground and chassis ground (the chassis is the higher potential). Reads –3.5mV with the leads reversed, which is expected behavior.

However: measuring resistance between the two grounds, I get 30–45Ω with the multimeter's red lead on the chassis ground and the black lead on the logic ground (the value varies somewhat). I get 0.000Ω when the leads are reversed. Perhaps this is because there's a current flowing, and when that current agrees with the multimeter's test charge, the meter reports no resistance, and when it's flowing against the test charge, the meter interprets it as resistance.

Measuring directly for current flow returns no value other than 0.0µA, with leads in either direction. The ~100µA I calculated above does not appear.

In conclusion: I still don't understand.

### Seeking a new piece of hardware to drive the LEDs

Figuring that perhaps there's a simple piece of hardware to drive the Big Digit LED segments, I searched in Digi-Key and Mouser to no avail, and had a conversation with a Digi-Key technical rep. We didn't find any part that would work for the application (the extant parts on the market tend to drive *far* fewer milliamps than these big segments require).

## 6/3/18 an hour

Continued email correspondance with Evil Mad Scientist about trying to troubleshoot; [posted a question to an electronics forum](https://electronics.stackexchange.com/questions/377976/seeking-a-high-current-common-anode-led-display-driver) since I wasn't able to find any in Digi-Key or Mouser catalogs. Heard back from a forum user, who suggested the TI chip referenced below.

Heard back from Betlux (the LED manufacturer) about power requirements for the displays, which weren't quite clear from the [datasheet](http://betlux.com/product/LED_seven_segment_display/big_segment_LED_display/BL-SE1200A.PDF). The engineer there said that each segment of the display should be run with 20-80mA regulated current, at 30–33V.

I had been considering replacing the BDD driver with a different part, TI TLC5960 ([datasheet](http://www.ti.com/lit/ds/symlink/tlc5916.pdf)), which *can* drive the needed current—but only at up to 20V. So unfortunately it won't work, and I don't believe any commercially manufactured part will.

## 6/5/18 6:45–9:45 p.m.

### Still finding no particular driver problem

Started by returning to logic analysis on the BDD driver board. I didn't find any voltages or currents outside of nominal, and, returning the BDD's V_LED supply to the cabinet's built-in supply (rather than the bench supply), I again didn't see any meaningful amount of flickering in the display.

### Cleaning up, securing wires, removing old control system

I decided to move on to cleaning up the old control system and wiring harnesses, and got fairly far through cleaning up wiring/routing on the TRUE side of the sign. I even took before and after pictures! (And forgot to copy them to the computer. Will do when I'm back in the space and put them here.)

I added many zip ties, screwed into the aluminum frame, to act as good long-term wire harnesses. I'm wary of small pieces of adhesive for holding things in place—eventually, small–surface area adhesives seem to all fail—and I hope these attachment points will have much more longevity. I also affixed the second marquee light attachment connector, replacing the old one.

Plenty more mechanical/wiring work to do here. Next up: the new control Nano which will just run the marquee pattern.

## 6/9/18 2:45–7:30 p.m.

### Marquee control replacement

Loaded a subset of the old capSign sketch onto an Arduino Nano so that it can run the marquee animation. (This Arduino will have only that job, and does not have any logical communication with the rest of the system because it's not needed.) Tested that and it's working properly, driving the four solid-state relays as expected, in the same pattern they used to operate.

The old sign electronics weren't so pretty:

<img src="/images/signElectronicsBefore.JPG"/>

I did a bit of soldering on some protoboard to make the new, small marquee animator:

<img src="/images/signElectronicsAfter.JPG"/>

I will be sealing up that box once I've made an airtight pass-through for the cable bundle, but for now, it's tested and working properly. The software running on the Arduino is `capMarqueeV18`, commit `250e985` on the `v18dev` branch. This works well and I left it running for a while as a test without trouble.

The buttons on the side of the box: if you push one of them, the marquee goes into "all-on" mode, which simply turns all the lights on constantly. This is useful, I figure, for testing the bulbs to make sure they're all working, or if you want to take a picture of the sign while the lights are all on. The other button puts the Arduino into "regular marquee" mode, which follows precisely the same pattern as the marquee did before modification.

### …and a stumble backwards

Resuming work on the large seven-segment display, I had far worse data corruption than I'd seen before. Simply plugging in the Arduino and turning on the lights, they started a stochastic flicker party across every not-burned-out segment. 'Surely this is just running some wonky test sketch, sending bad data' I told myself, and uploaded the latest version again. Nope, that was what was actually happening. Still an electronic issue, I'm convinced: when I attach the oscilloscope's negative probe to the logic probe, suddenly the circuit calms a great deal and the flickering reduces significantly. Note that the logic ground is floating, not connected at all with the chassis, so that shouldn't be the source of the problem.

I wondered, does the Arduino's 12V DC supply have a very different voltage than the big LEDs' 36V DC supply? Yes, when floating, it definitely does. In fact, using the scope I saw a huge swing at 60Hz between the two of them: something like 60V peak to peak.

I heard from Steve Lambert that the Australian iteration of the project uses the [Sparkfun Large Digit Driver](https://www.sparkfun.com/products/13279), which is really just a carrier for the TI [TPIC6C596](http://www.ti.com/lit/ds/slis093d/slis093d.pdf). These are available in a DIP package, mercifully. I'm going to order some with fast delivery and see if I can substitute these, along with some bias resistors, to replace the Evil Mad Scientist Big Digit Drivers. They are tolerant of up to 33V, and have 8 current sink outputs at up to 100mA each, so they're within specs for the application. I just don't think it's worth spending more time continuing to try to find the electrical problem in the BDDs if there are viable alternatives available, and it's really worth doing the experiment at this point.

(Fortunately, the seven-segment supply has a fine-tuning pot on it and is able to be turned down to below 33V, so I can be assured that the TPIC6C596 will never see a voltage outside of its rating, without needing a bias resistor for the VLED.)

## 6/11/18 

### 10:45–11:15 a.m.

Finalized and placed Digi-Key order to attempt to use new drivers (TPIC6C596N), in the likely event that I decide I cannot reliably solve the problem with the old ones.

### 6:45 p.m.–1 a.m.

A final diagnostic attempt before giving up on my ability to reliably control, with only very occasional flickering of any sort, the big digits.

Following Garth's advice, I desoldered a Zener diode on the Big Digit Driver board so that I could use substitute my own clean power supply from a regulated source, instead of using the VLED source but stepped down: the theory being that ongoing noisy voltages between VLED's ground and the Arduino's ground (which were tied together) was creating signalling trouble on the SPI lines.

With my own 16V regulated power flowing to the BDD's digital supply, I observed no flickering at all in the digits. The communication problem, therefore, may be solved by driving the BDD's digital supply with the Arduino's VIN power, coming from the Arduino's own 12V DC regulator. I will attempt that next. But one possible bit of trouble before I do so: this first modification only affects the first digit of the series of three—the following two digits down the chain are still getting their logic power from the VLED supply, stepped down. Regardless, I haven't seen flicker, so it may be that only the first in the sequence needs to be fixed.

---

I installed 5.5mm input jacks on the three large electronics enclosures I'm adding (podium and two digit drivers). This is part of the electrical/mechanical preparation for completing the new installation work.

---

Running the Arduino DC regulator's power straight to the BDD's power input at the pre-regulation stage does not seem to have solved the problem, unfortunately. It's still somewhat noisy; adding a 100µF 50V capacitor I had laying around across the 12V DC input quiets the noise significantly and appears to reduce the seven-segment flicker, but it's still far more than is acceptable.

In a 60 second sample, where the Arduino is sending an SPI update at 1Hz, I count 38 flickers (most of which are at 1s intervals, as the updates are pushed). Adding the filter capacitor as the Arduino's 12V DC power enters its enclosure, I did another 60s test and counted 6 flickers. This seems confirmatory of the dirtiness of the BDD's logic supply being the crux of the problem.

---

I wired the FALSE side's Arduino controller to the BDD unit, and also am feeding 12V from the Arduino DC converter straight into its logic circuit, right in front of its voltage regulator, just as I did with the TRUE side.

Upon powering up FALSE, it is obvious that it has many burned out segments (out of the 7&times;3 which could be illuminated, at least 5 are not on), however it is not blinking/flickering at all, quite unlike the TRUE side, which (despite leaving the smoothing caps in place) has now resumed blinking nearly once per second.

It's worth noting that this condition is while the two chassis of the halves of the sign are electrically isolated. I will see if I notice any change upon connecting them with a wire.

Inconsistent results. At first, on the scope I observed a fairly large-amplitude noisy sawtooth signal between the two chassis: a p–p swing of ~5V. Then, I rearranged my test wires slightly and subsequent to that haven't seen a signal more than ~1.5V p–p.

I added two lines to `capDigitDriverV18` so that the sign halves would display `888` for 5 seconds upon startup. This appears in commit `b62f126` on the `v18dev` branch. I uploaded this to the TRUE side, taking care not to touch my laptop chassis to the metal of the sign while plugged in via USB, because then I'd be grounding the logic to the chassis. Upload done, I unplugged, and the TRUE side began by writing `888` as desired for 5 seconds, followed by stochastically lighting every segment, a total flicker which now persists across restarts. Meanwhile, the same new firmware upload to the FALSE side produces no unexpected flickering. Further evidence of subtle, difficult-to-reproduce ground trouble. Frustrating. Packing it in for the night.

Noise observed between the chassis halves; note the dominant bumps at a period of ~16µs, a corresponding frequency of 62.5kHz. I don't know what's causing it.

<img src="/images/62kHz_signal_between_chassis_halves.jpg">

## 6/15/18 6:15–9:30 p.m.

Using a spare Big Digit unit to test for its behavior when being driven right around its V_F. The circuit is very simple: a voltage-regulated source providing 29–32V, in series with a 33Ω resistor to act as a current limiter, in series with a single LED segment of the digit.

I'm on the steep part of the V–I curve, but if I am careful not to apply too much voltage, the current should stay within appropriate bounds. 

Observations of different segments, where I is a function of the V supplied in the above described test circuit:

LED segment | V (V) | I (mA)* | light output (subjective)
---|---|---|---
A | 29 | 20 | dim
" | 30 | 30 | medium
" | 31 | 50 | bright
" | 32 | 70 | brighter
F | 29 | 20 | dim
" | 30 | 30 | medium
" | 31 | 50 | bright
" | 32 | 70 | brighter
G | 29 | 20 | dim
" | 30 | 30 | dim
" | 31 | 50 | medium
" | 32 | 70 | bright
E | 29 | 20 | dim 
" | 30 | 30 | medium
" | 31 | 50 | bright
" | 32 | 70 | brighter

*The ammeter I'm using seems for some reason to be fairly high-impedance in mA measurement mode, so I have to use the 10A mode; I have a resolution of 0.01A in this mode, hence the round numbers.

The manufacturer recommends current-limiting the LED dice at 5–20mA each, and they are in 4-parallel strings, so each segment should be run at 20–80mA; the 50mA that these segments all appear to settle at when being driven at 31V is comfortably within the range and gives plenty of brightness. It also gives the TPIC6C596N two volts of headroom below its absolute maximum V_DS of 33V. Based on this data, which is confirmatory of my hunch, I think running the V_LED at 31V is a reasonable compromise. I retested each segment's brightness at 31V and they all look good.

Next, I will breadboard my TPIC6C596N circuit to test and debug driving a single digits' segments. If it works, I'll test two segments in a chain (to address the problem of data transmission between the ICs), and if that works, I'll deinstall the current BDD hardware to install my TPIC6C596N replacement on one half of the sign.

---

I've breadboarded and gotten the TPIC6C596N to drive all 7 segments of a single digit, at a steady brightness. As I've hit some software trouble in controlling the digit (a simple problem involving how I'm shifting data out, as far as I can tell), I've noticed a worrying trend going on in the background. The ammeter on my power supply has been very slowly creeping up as the digit has been sitting on 8 for about 30 or 45 minutes by now. It began at ~370mA (which is ~53mA per segment, no problem), but now has crept up to ~400mA, or ~57mA per segment). If this trend continues, it'll be worrisome because my ballast resistors rely on a fairly steady V_F.

A good sign is that the TPIC6C596N, now sinking ~400mA for close to an hour, remains cool to the touch.

Tomorrow, I will resume this work, including paying close attention to the current draw of a segment that I leave wired on for consecutive hours.

Image: segment running, with dual power supply in the background. The left supply is running the LED and the right supply is the Arduino's and IC's logic supply, before running through a 7805 regulator.

<img src="/images/TPIC6C596_driver_test.jpg">

## 6/16/18 

### 11:30 a.m.–1 p.m.

I have to investigate that current creep problem. It may have been some sort of burn-in in the LEDs or driver chip, or perhaps it had to do with the temperature of a component rising over time. As I'm working I'm going to record current over time as reported by the power supply's ammeter; I'll record changes as I notice them. This is still running at 31V, voltage regulated.

time | elapsed (m) | current (mA)
---|---|---
11:44 | 0 | 320
11:45 | 1 | 330
11:45 | 1 | 340
11:47 | 3 | 350
11:49 | 5 | 360
11:52 | 8 | 370
12:00 | 16 | 380
12:05 | 21 | 390
12:18 | 34 | 400
12:44 | 60 | 400
13:00 | 74 | 400
14:06 | 142 | 400
15:00 | 196 | 400

I wired up a second TPIC6C596N while observing these values, taking time to cut leads and resistor legs to good lengths since I will reuse them all when transferring the breadboard wiring to the protoboards.

### 2:15–4 p.m.

Resistor value calculations, which I did earlier but didn't write up:

V=iR

V = V_LED - V_F

for V_LED of 31V, i of 0.04A, and V_F 30V

(V_LED - V_F) ÷ i = R
1V ÷ 0.04A = 25Ω

P = iV = 0.04A * 1V = 0.04W power dissipation, so a 1/4W resistor is certainly fine.

I'm using **33Ω** resistors in series with each LED segment to err on the side of a bit more current regulation.

---

Viewed from the top, the pads on each full digit numeral, from left to right, are

A F G E (V_LED) D C B (decimal point)

The rightmost pad is the decimal point which need not be soldered in this project. The common positive (anode) V_LED pad is substantially thicker than the others, which makes sense as it carries seven times the current.

___

Still having significant trouble communicating with the chip. I'm getting segments lighting up, but I can't reliably relate the pattern of digits to the bits I'm writing out with my serial commands. Not worth writing up the details but lots of trials and so far, lots of errors.

# master list of issues:

(this list from April, prior to the installation of the sign; repair work after deinstallation does not appear here but is detailed in the work log above)

status | description | materials | comments
--- | --- | --- | ---
fixed 4/13 (hopefully—5/1 upon installation, some may have burned out. TBD.) | some individual segments in the large 7-segment displays don't work | replacement segments are included in the project's traveling package | the reason I was brought on in the first place was to repair this known problem. It *may* be related to some sort of grounding fault, since apparently separating the two halves of the sign may solve it. I haven't gotten far enough along to meaningfull diagnose
complete 4/26 | electronic connector trouble | ordered a new 12-circuit waterproof connector to replace current connectors | Old connectors may well have been working fine, but this replacement will allow me to add in a significant length of extra wire so that the sign segments can be spread apart from each other during repair.
obviated 4/13 | communication trouble | new female–female ethernet connector (on order) | it appears that the podium is not successfully sending data to the sign. May be a connector issue, as per testing on 4/6/18.
5/1 one half of the sign has a new connector; the other half will be replaced with the same type as well | power to "CAPITALISM" marquee is dangerous | new 6-conductor connectors | There are four solid-state relays that drive the lights in the marquee part of the sign. When I opened the cabinet, these stranded-copper wires were just dangling in the air, uninsulated…I poked around with a multimeter and found that they were putting out 120VAC at different intervals. Not so good! These need to go into a proper high-ish current connector instead of floating. As of 4/26 I have the connectors and will acquire wire nuts to attach the sign-side plug.
deemed unnecessary to work on 4/13 | wireless communication? | -- | it appears that there are two indpendent connections, one wired via ethernet cable and one wireless. The wireless side isn't functioning. This may not matter, since they may simply be redundant, but I'm not clear on that yet.
fixed 4/26 | number errors | -- | the eeprom values are initializing at the maximum `unsigned long` value instead of 0, with my nearly-unmodified upload of the same firmware the thing was running before I touched it. I don't know what difference my software is making that affects this.
complete 4/29 | chassis grounding | grounding pigtails, bus connection posts | potentially related to the first problem listed (7-segment display trouble): neither of the metal chassis of either bottom half of the signs are electrically attached to their own local ground, and there may be a ground loop or other problem when they're mechanically attached. I am affixing grounding points into both chassis which will connect to the electronic ground on both sides so that all grounds are tied together correctly.
mostly done 4/29 | mounting wires and electronics | zip ties, self-tapping screws | many wires are floating around inside the signs and should be better anchored mechanically.

# replacement parts purchase accounting

date | vendor | item | quantity | explanation | line total cost
---|---|---|---|---|---
4/25 | McMaster-Carr | 6-conductor wire connector (part 9552T2) | 1 pair | to replace old connectors, running 120V to CAPIT | $11.62
4/25 | McMaster-Carr | shipping ||| $5.84
4/26 | Home Depot | wire nuts assortment | 1 bag | for rewiring 120V terminals | $4.87
4/26 | Home Depot | 3-prong plug | 1 | to tie chassis ground to wired ground | $2.79
4/26 | Home Depot | ground bar | 2 | to make ground bus in sign and podium | $6.56
4/26 | Home Depot | pan head stainless screws, self-drilling, #8, 3/4" length | 1 box | for making new physical mounting points in sign chassis | $5.21
4/26 | Home Depot | 16 AWG green stranded wire | 24' | for adding ground paths in sign chassis | $5.55
4.26 | Home Depot | cable ties with mount hole, 8" length | 100 pack | for various chassis affixing | $8.88
4/26 | Home Depot | grounding pigtails | 5 pack | for adding ground points to sign and podium chassis | $3.27
4/26 | Home Depot | sales tax ||| $2.60
5/7 | OSH Park | custom PCB for new logic boards | 3 | see design files for details | $65.50
5/7 | OSH Park | express shipping ||| $24.70
5/9 | Digi-Key | 2-position terminal block, 0.2" spacing (part 1920-1323-ND) | 25 | terminals on new logic boards | $11.35
5/9 | Digi-Key | RS-485 half-duplex transmitter/receiver (part SN75HVD3082EP) | 4 | RJ-45 communication between new logic boards | $10.52
5/9 | Digi-Key | power jack connector (part CP-037A-ND) **note: wrong part! substituted with found parts from lab instead** | 4 | barrel jack to bring 12V power to new logic boards | $2.32
5/9 | Digi-Key | 5V power regulator (part 296-39515-5-ND) | 4 | regulate 12V down to 5V for Arduino Nano and other logic on new boards | $2.32
5/9 | Digi-Key | board-mount ethernet jack (part AE10384-ND) | 4 | connection point for RJ45 cable to new logic boards | $2.76
5/9 | Digi-Key | 8 position DIP socket (part AE9986-ND) | 4 | to seat RS-485 chip on new logic boards, removably | $0.72
5/9 | Digi-Key | 15-position female header (part S7048-ND) | 4 | to seat Arduino Nano on new logic boards, removably | $4.08
5/9 | Digi-Key | shipping and sales tax ||| $9.94
5/14 | McMaster-Carr | 6-conductor wire connector (part 9552T2) | 1 pair | to replace old connectors, running 120V to ALISM | $11.62
5/14 | McMaster-Carr | 4-conductor wire connector (part 9552T4) | 2 pair | to replace old connectors running 36V to 7-segment LEDs and white illumination behind FALSE | $18.00
5/14 | McMaster-Carr | shipping ||| $5.85
5/14 | Amazon | [12V 2A power supply](https://www.amazon.com/gp/product/B019Q3U72M) | 3 | to supply power to new logic boards (one in podium; one by FALSE; one by TRUE) | $23.97
5/14 | Amazon | 12' white extension cord | 2 | to run 120V power from center of sign to TRUE and FALSE sides for their 12V transformers | $5.79
5/14 | Amazon | [240mmx160mmx90mm ABS enclosure with clear face](https://www.amazon.com/gp/product/B07122FQQ7) | 3 | to contain and protect new electronics at podium, by FALSE, and by TRUE | $59.97
5/14 | Amazon | three-way RJ45 (ethernet) splitter |1 | to allow one podium signal to break into FALSE and TRUE signals | $6.22
5/16 | McMaster-Carr | #8 steel washers (part 98023A113) | pack of 100 | needed for crate/packing hardware | $7.54
5/16 | McMaster-Carr | countersunk washers (part 98466A009) | pack of 100 | needed for crate/packing hardware | $4.09
5/16 | McMaster-Carr | 5/16"-18 nuts (part 94895A030) | pack of 100 | spares needed for crating/packing | $5.12
5/16 | McMaster-Carr | washers for 5/16" bolts | pack of 100 | spares needed for crating/packing | $5.10
5/16 | McMaster-Carr | shipping ||| $5.95
to add: 5/26 Amazon order, total $55.13
5/29 | Amazon | clear-face electronic junction box | 1 | for marquee-driving Arduino | $8.99
5/29 | Amazon | Arduino Nano | 5 | podium, TRUE side, FALSE side, and marquee (plus an extra) | $20.99
5/29 | Amazon | tax ||| $1.80
6/11 | Digi-Key | TPIC6C596N IC | 7 | new LED driver for big digits | $10.36
6/11 | Digi-Key | part 1175-1477-ND 16-DIP socket | 7 | mount ICs to protoboard | $1.68
6/11 | Digi-Key | SBB830-ND full-size protoboard | 2 | for new electronics mounting on TRUE and FALSE sides | $12.98
6/11 | Digi-Key | HS179-ND clip-on heatsink | 7 | to heatsink ICs | $5.18
6/11 | Digi-Key | shipping ||| not yet known
**sum** | ||| as of 6/11, without Digi-Key shipping | **$422.60**
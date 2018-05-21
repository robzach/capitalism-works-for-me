My work log for repairing Steve Lambert's [*Capitalism Works for Me*](https://visitsteve.com/made/capitalism-works-for-me-truefalse/) piece for inclusion in the [Marx@200 show](http://www.spacepittsburgh.org/portfolio-view/marx200/) organized by CMU Profs. Kathy Newman and Susanne Slavick. Repair work carried out in April 2018 and installation completed on May 1st.

# master list of issues:

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

## Hours worked up to this point: 61.5 hours

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
**sum** | ||| as of 5/21 | **$322.82**
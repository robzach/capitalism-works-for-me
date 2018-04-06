My work log for repairing Steve Lambert's [*Capitalism Works for Me*](https://visitsteve.com/made/capitalism-works-for-me-truefalse/) piece for inclusion in the [Marx@200 show](http://www.spacepittsburgh.org/portfolio-view/marx200/) organized by CMU Profs. Kathy Newman and Susanne Slavick.

# master list of issues:

status | description | materials | comments
not yet worked on | some individual segments in the large 7-segment displays don't work | replacement segments are included in the project's traveling package | the reason I was brought on in the first place was to repair this known problem. It *may* be related to some sort of grounding fault, since apparently separating the two halves of the sign may solve it. I haven't gotten far enough along to meaningfull diagnose
in process | connector trouble | ordered a new 12-circuit waterproof connector to replace current connectors | Old connectors may well have been working fine, but this replacement will allow me to add in a significant length of extra wire so that the sign segments can be spread apart from each other during repair.
in process | communication trouble | new female–female ethernet connector? | it appears that the podium is not successfully sending data to the sign. May be a connector issue, as per testing on 4/6/18.

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

## 4/6/18 1 p.m.–: communication debugging, documentation

I suspect communication trouble over the current CAT5 wire that's set up.

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

* One connector, which I've since labeled "iffy!", produces lots of <pre>data received from podium: 0</pre> lines when wiggled.
* The other connector, which I've labeled "good?", produces no output at all when wiggled. That seems much better to me.

Obviously, these wires shouldn't expect to be actively jostled while the piece is installed. But the fact that the data isn't getting through, and that one of the connectors looks wonky, makes me think it could be part of the problem.


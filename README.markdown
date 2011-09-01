BADGE WARS
==========

*Assembly language meets Battlebots*
------------------------------------

Two warriors enter the arena. These warriors are not flesh and blood, they are
made out of a series of instructions written in a language called Redcode.
It's a battle of bits to the death! Only one will survive. The winner will go on
to be king of the core.

BadgeWars is an implementation of [CoreWars](http://www.corewars.org), but a
minified version intended for use on microcontrollers. Microcontrollers are
everywhere nowadays, and one of the most interesting places they're showing
up are on conference badges, specifically badges at [Defcon](http://defcon.org).

BadgeWars aims to provide a distributed CoreWars ecosystem where each badge
has a "king of the core" (KOTC). Badges communicate over mesh wireless, 
exchanging KOTCs with each other. If a newcomer manages to outlive the badge's
current KOTC, it becomes the KOTC and can start replicating to other nearby
badges, at least as long as it remains KOTC.
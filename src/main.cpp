#include "Editor.hpp"


/*
event preconditions:
e: seen event _
h: don't have pet, and Farmer.catPerson == _
m: total money earned >= _
M: current money >= _
c: free inventory space >= _
C: seen 191393 || seen 502261 || completed CC
D: dating _
j: played days > _
J: "checkJojaCompletePrerequisite()"
f: "checkFriendshipPrecondition(_)"
F: today is not a festival day
r: rand.NextDouble() <= _
s: "checkItemsPrecondition(_)"
q: ^, dialogue
n: mail _ received
l: mail _ not received
t: time >= _1 && time <= _2
w: 'rainy' and raining || 'sunny' and not raining
d: not one of these week days
o: _ is not your spouse
v: _ is not invisible (discovered?)
p: 'isCharacterHere' _
z: season is not _
b: player.timesReachedMineBottom >= _ (never increased in vanilla)
y: year >= _ and (_ != 1 || year == 1)
g: gender is 'male' or 'female'
i: player has item _ || holding.ParentSheetIndex == _
k: 'checkEventsSeenPreconditions()'
a: player is at x,y
x: adds _ as mail tomorrow and sets current event as scene, but is still a failing condition
u: is one of these numerical days of the month
*/

int main( int argc, char* argv[] )
{
    Editor editor( argc, argv );
    editor.run();
}

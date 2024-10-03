## Enable/disable different parts 

**instrument config**
```
instrumentSection: guitar1.A guitar1;
instrumentSection: guitar1.B guitar1;
```

**disable A.conductions**
```
instrument(guitar1.A) {
  audibility: silent;
}
```

**disable B.conductions**
```
instrument(guitar1.B) {
  audibility: silent;
}
```

**Main.sheet**
```
-- using "./disable A.conductions";
using "./disable B.conductions";
```

## Grace Note alternative (more options)
```
(cb16 c2)4
```


## drum variations

"mute" and "cyOnOne" implements Crash on one after fill.
```
{
  "mute"@<q, q>4
  /endTag: cyOnOne/ /endTag: cy2OnOne/
  x r x |

  ^1 r4 x r x :|
  ^2 r4 x r x8 !ffffq /beginTag: cy2OnOne/ :|
  ^3 r4 x r x :|
  ^4 r4 x "2x paradiddle"@<x 'x h h>2 /beginTag: cyOnOne/ :|

  ^5 r4 x r x :|
  ^6 r4 x r x8 !fffffh' :|
  ^7 r4 x r x :|
  ^8 r4 x "2x paradiddle"@<x 'x>2 /beginTag: cyOnOne/ :|
  
  ^9 r4 x r x :|
  ^10 r4 x r x8 !fffffh' :|
  ^11 r4 x r x :|
  ^12 r4 x (x' x x' q)4 (x x' x q )4 |
}
```

**drum conductions**
```
instrument(drums) withTag(mute) {
    velocity = 0;
}


instrument(drums) pitch(q) withTag(cyOnOne){
    velocity = 120;
}

instrument(drums) pitch(q,) withTag(cy2OnOne){
    velocity = 120;
}

```


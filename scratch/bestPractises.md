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

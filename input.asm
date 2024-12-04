;DATA
array:
data 143
data 75
data 9
data 45
data 98
data 67
data 23

length:data 7

counter:data 0

position:data 0
 
;CODE

main:
;CHANGING LENGTH TO ZERO INDEX
ldc length
a2sp
ldl 0
adc -1
stl 0
br bubble

bubble:
;TERMINATION CONDITION
ldc length
a2sp
ldl 0
ldc counter
a2sp
ldl 0
sub
brz end
br sort

;SORT FUNCTION
sort:
ldc position
a2sp
ldl 0
ldc length
a2sp
ldl 0
sub
brz reset

ldc position
a2sp
ldl 0
ldc array
add
a2sp
ldl 0

adj 1
ldl 0
sub
brlz iterate

;SWAP
swap:
ldl -1
ldl 0
adj -1
stl 0
adj 1
stl 0

;ITERATE
iterate:
ldc position
a2sp
ldl 0
adc 1
stl 0
br sort



;RESETS POSITION TO ZERO AND UPDATES COUNTER
reset:
ldc position
a2sp
ldc 0
stl 0
ldc counter
a2sp
ldl 0
adc 1
stl 0
br bubble


end:
HALT


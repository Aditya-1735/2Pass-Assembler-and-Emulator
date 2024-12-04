;DATA
array:
data 16 ;16
data 4
data 56 ;070
data 2
data 1
data 6

length:    data 6       ; Number of elements in the array
key:       data 070       ; Value to search for
index:     data -1       ; Resulting index (-1 if not found)
counter:   data 0      ; Loop counter


;CODE
main:
; CHANGING LENGTH TO ZERO-INDEXED
ldc length              ; Load the length of the array
a2sp
ldl 0
adc -1               ; Adjust length to zero-indexed
ldc index
a2sp
stl 0


; INITIALIZE COUNTER TO 0
ldc counter
a2sp
ldc 0
stl 0


; INITIALIZE INDEX TO -1 BEFORE STARTING SEARCH
ldc index
a2sp
ldc -1
stl 0
br search            ; Jump to the search routine


search:
; TERMINATION CONDITION: IF COUNTER == LENGTH, EXIT LOOP
ldc counter          ; Load the counter
a2sp
ldl 0
ldc index           ; Load zero-indexed length
a2sp
ldl 0
sub                  ; Compare counter with length
brz end              ; If counter == length, key is not found

; ACCESS CURRENT ARRAY ELEMENT
ldc counter          ; Load the counter
a2sp
ldl 0
ldc array            ; Get the base address of the array
add                  ; Calculate the address of array[counter]
a2sp
ldl 0                ; Load array[counter]

; COMPARE ELEMENT WITH KEY
ldc key              ; Load the key
a2sp
ldl 0
sub                  ; Compare array[counter] with key
brz found            ; If equal, jump to found


; INCREMENT COUNTER
ldc counter          ; Load the counter
a2sp
ldl 0
adc 1                ; Increment counter
stl 0
br search            ; Repeat the search



found:
; STORE CURRENT INDEX IN INDEX
ldc counter          ; Load the counter (index of the found element)
a2sp
ldl 0
ldc index            ; Load the address of the index
a2sp
stl 0                ; Store counter in index
br end               ; Exit
end:
HALT                 ; Stop execution
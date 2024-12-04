a:
data 1
data 2
data 59
data 66
data 23
data 1
data 10

i:data 0
total:data 6
max:data -1

while:
    ldl total;
    ldl i;
    sub
    brlz end
    ldl i;
    ldnl 0
    ldl max;
    sub
    brlz nochange
    ldl i;
    ldnl 0
    stl max

nochange:
    ldl i;
    adc 1
    stl i;
    br while
    
end:HALT
jmp fun_4
fun_0:
pop [0]
pop [1]
push 0
push [1]
push [0]
div 
sub 
pop [2]
push [2]
ret
fun_1:
pop [0]
pop [1]
pop [2]
push [1]
push [1]
mul 
push 4
push [0]
mul 
push [2]
mul 
sub 
pop [3]
push [3]
ret
fun_2:
pop [0]
pop [1]
pop [2]
push 0
push [1]
sub 
push [2]
sqrt 
add 
push 2
push [0]
mul 
div 
pop [3]
push [3]
ret
fun_3:
pop [0]
pop [1]
pop [2]
push 0
push [1]
sub 
push [2]
sqrt 
sub 
push 2
push [0]
mul 
div 
pop [3]
push [3]
ret
fun_4:
in
pop [0]
in
pop [1]
in
pop [2]
push [0]
push 0
jne end_if_0
push [1]
push 0
jne end_if_1
push [2]
push 0
jne end_if_2
push 111
pop [3]
push [3]
out
end_if_2:
push [2]
push 0
je end_if_3
push 222
pop [3]
push [3]
out
end_if_3:
end_if_1:
push [1]
push 0
je end_if_4
push [2]
push [1]
call fun_0
pop [3]
push [3]
out
end_if_4:
end_if_0:
push [0]
push 0
je end_if_5
push [2]
push [1]
push [0]
call fun_1
pop [4]
push [4]
push 0
jne end_if_6
push 0
push [1]
sub 
push 2
push [0]
mul 
div 
pop [3]
push [3]
out
end_if_6:
push [4]
push 0
jbe end_if_7
push [4]
push [1]
push [0]
call fun_2
pop [3]
push [3]
out
push [4]
push [1]
push [0]
call fun_3
pop [3]
push [3]
out
end_if_7:
push [4]
push 0
jae end_if_8
push 222
pop [3]
push [3]
out
end_if_8:
end_if_5:

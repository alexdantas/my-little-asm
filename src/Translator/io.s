;; Generated with `translator`
;; Do not edit manually!
;;
;; Input-Output procedures
;; (not part of the original source)

; BEGIN OF INPUT/OUTPUT PROCEDURES
;;; Also, tells how to make procedures

;; Reserving space for le buffer
		section .bss

buffer:		  resb	256

		section .text

;;; Clears buffer's contents
zero_buffer:
		pusha

		mov ecx, 0				; i = 0

zero_buffer_loop:
		mov BYTE [buffer + ecx], 0

		add ecx, 1				; i++

		cmp	ecx, buffer_max_size
		jl	zero_buffer_loop	; continue if (i < buffer_max_size)

zero_buffer_end:
		mov [buffer_size], ecx	; current_size = max_size

		popa
		ret

;;; Shows an end-of-line on standard output.
show_endline:
		pusha					; Saves all major registers

		mov eax, 4				; Show string
		mov ebx, 1				; on stdout
		mov ecx, endl			; Char
		mov edx, 1				; Size
		int 80h					; Do it!

		popa					; Restore all major registers
		ret						; Return to calling context

;;; Reads a string from standard input on `buffer`,
;;; storing it's size on `buffer_size`.
read_string:
		call zero_buffer
		pusha

		mov eax, 3				; Read string
		mov ebx, 0				; on stdin
		mov ecx, buffer			; Where to store
		mov edx, buffer_size	; Maximum bytes to read
		int 80h					; Do it!

		sub eax, 1				; Ignoring trailing '\n'
		mov [buffer_size], eax	; Number of bytes read

		popa
		ret

;;; Shows `buffer` on standard output.
show_string:
		pusha

		mov eax, 4				; Show string
		mov ebx, 1				; On stdout
		mov ecx, buffer
		mov edx, buffer_size
		int 80h

		popa
		ret

;;; Returns the number of digits on ESI.
;;; Stores it on ECX
int_digits:
		push eax
		push edx
		push ebx

		mov eax, esi
		mov ecx, 0				; digits = 0

int_digits_loop:
		mov	 edx, 0
		mov	 ebx, 10
		idiv ebx				; num = num / 10

		add	 ecx, 1				; digits++
		cmp	 eax, 0				; if (num != 0) continue
		jne  int_digits_loop

		pop ebx
		pop edx
		pop eax
		ret

;;; Converts integer number on ESI to string on `buffer`
;;; with `buffer_size`.
int_to_ascii:
		call zero_buffer

		pusha
		push esi

		mov eax, esi			; num = number_to_convert

		call int_digits			; i = number_of_digits(num)

		cmp	eax, 0
		jl	int_to_ascii_negative ; if (num < 0) goto negative
		mov edi, 0
		jmp int_to_ascii_loop

int_to_ascii_negative:
		mov edi, 1
		add ecx, 1 				; the minus sign at the start

		mov eax, esi			; Multiplying by -1 to treat
		mov esi, -1				; as positive
		mul esi

int_to_ascii_loop:

		;; mov	 esi, eax

		mov	 edx, 0
		mov	 ebx, 10
		idiv ebx				; num = num / 10

		mov BYTE [buffer + ecx], dl		; string[i] = num % 10
		add BYTE [buffer + ecx], 30h	; string[i] += '0'

		sub	ecx, 1				; i--

		cmp eax, 0				; if (num != 0) continue
		jne int_to_ascii_loop

int_to_ascii_end:
		call int_digits
		mov [buffer_size], ecx	; buffer_size = number_of_digits

		cmp edi, 1
		jne int_to_ascii_non_negative_loop_end

int_to_ascii_negative_loop_end:
		mov BYTE [buffer], 2Dh	; '-'

int_to_ascii_non_negative_loop_end:

		pop esi
		popa
		ret

;;; Converts ASCII number on `buffer` with `buffer_size` to
;;; integer on ESI.
ascii_to_int:
		push eax
		push ebx
		push ecx
		push edx

		mov bl, BYTE [buffer]	; Negative numbers
		cmp bl, 2Dh				; '-'
		jne	ascii_to_int_non_negative


ascii_to_int_negative:
		mov edi, 1				; FLAG

		mov ebx, 0
		mov bl, BYTE [buffer+1]	; buffer[1]
		sub bl, 30h				; buffer[1] - '0'

		mov	esi, ebx			; num = buffer[1] - '0'

		mov ecx, 2				; i = 2
		jmp ascii_to_int_loop

ascii_to_int_non_negative:
		mov edi, 0				; FLAG

		mov ebx, 0
		mov bl, BYTE [buffer]	; buffer[0]
		sub bl, 30h				; buffer[0] - '0'

		mov	esi, ebx			; num = buffer[0] - '0'

		mov ecx, 1				; i = 1

ascii_to_int_loop:
		cmp ecx, [buffer_size]		; if (i == buffer_size) break
		je  ascii_to_int_end

		mov eax, esi
		mov esi, 10
		mul esi
		mov esi, eax				; num *= 10

		mov bl, BYTE [buffer + ecx]	; buffer[i]
		sub bl, 30h					; buffer[i] - '0'

		add	esi, ebx				; num += buffer[i] - '0'

		add ecx, 1					; i++
		jmp ascii_to_int_loop

ascii_to_int_end:
		;; When calling internal functions sometimes
		;; the integer is off by one.
		;; Why?
		;; sub esi, 1				; num-- (why?)

		cmp edi, 1
		jne	ascii_to_int_non_negative_end

ascii_to_int_negative_end:
		mov eax, esi			; Multiplying by -1
		mov esi, -1
		mul esi
		mov esi, eax

ascii_to_int_non_negative_end:
		pop edx
		pop ecx
		pop ebx
		pop eax
		ret

;;; Reads an integer from standard input, storing on ESI
read_int:
		call read_string
		call ascii_to_int
		ret

;;; Prints an integer on standard output, that is on ESI
show_int:
		call int_to_ascii
		call show_string
		call show_endline
		ret
;; End Of Input-Output Procedures

global _start
_start:
		mov eax, 0


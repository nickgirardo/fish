.export _string_buf
.export _action_buf

.segment "CODE"

_string_buf:
	.byte "1EdI’m Ed! I’m one line of dialog!"

_action_buf:
	;; Action 0
	;; ActionEnd {}
	.byte 00			 ; Kind 0 = ActionEnd
	.byte 00, 00, 00, 00, 00, 00, 00, 00 ; Padding
	;; Action 1
	;; ActionDialogLine {{31, 3} {3, 3} 0}
	.byte 01			 ; Kind 1 = ActionDialogLine
	.byte 31, <(_string_buf+3), >(_string_buf+3)
	.byte 03, <(_string_buf+0), >(_string_buf+0)
	.byte <(_action_buf+0), >(_action_buf+0)
	;; Action 2
	;; ActionPortal {{0x60, 0x60, 0x60, 0x40, 0x40} 0}
	.byte 02			 ; Kind 2 = ActionPortal
	.byte $60, $60, $60, $40, $40
	.byte <(_action_buf+0), >(_action_buf+0)
	.byte 00		; Padding

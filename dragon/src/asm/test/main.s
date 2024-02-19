	; Sample code to clear the text screen
	org 20000

	ldx #1024
	lda #145

clear
	sta ,x+
	cmpx #1536
	bne clear
	rts



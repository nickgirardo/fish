.export _draw_tilemap_real
.importzp sp, tmp1, tmp2, tmp3
.import incsp2, _queue_end, _queue_count, pushax
.import _tilemap

DMA_flags = $2007

vram_VX = $4000
vram_VY = $4001
vram_GX = $4002
vram_GY = $4003
vram_WIDTH = $4004
vram_HEIGHT = $4005
vram_COLOR = $4007
vram_START = $4006

tile_size = $08
tilemap_width = $20

.segment	"DATA"

.proc	_draw_tilemap_real: near

.segment	"DATA"

	jsr     pushax

	;;
	;; SETUP
	;;

	;; Put the argument to the function in the y register
	tay

	;; Always start from y pos 0
	stz tmp2		; The current y position

	;; Calculate start x
	;; This determines how many pixels within a tile each tile should be adjusted by
	;; This value is initially stored in the x register and is incremented by `tile_size` after each draw
	and #tile_size - 1

	sta tmp1
	lda #tile_size
	sec
	sbc tmp1

	tax
	;; This writes into the argument of a ldx instruction
	;; This value is reused every time a new row is started
	sta RowStartingXPosParam+1

	clc
	adc #$10

	;; Retrieve the argument from the y register
	;; The y register can now be used for other purposes
	tya

	;; Calculate first x tile (tmp1)
	;; This is the location in the tilemap of where the first x tile should be drawn from
	;; That is to say, this value is used to index the tilemap
	lsr a
	lsr a
	lsr a
	clc

	adc #1

	sta tmp1

	;; Prep the dma flags
	;; TODO this value might be wrong, it's copied from the value given by the C code
	lda #$dd
	sta DMA_flags

	;; Set initial VRAM values
	;; The GX and GY registers are for textured draws and will remain 0 for this procedure (for now)
	stz vram_GX
	stz vram_GY
	;; Zero Y for the first draw
	stz vram_VY
	;; The width and height are set to `tile_size` for all draw calls
	lda #tile_size
	sta vram_WIDTH
	sta vram_HEIGHT

	;; Skip checking if finished and incrementing y on first draw
	bra StartFirstDraw

	;; The following vram values need to be changed on each row
StartOfRow:
	;; Enough nops to make sure we finish the previous draw
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop

	;; Each time we finish a row, check if we've drawn enough tiles
	lda tmp2
	cmp #$80-tile_size
	bpl End

	;; Increment the current y pos by `tile_size`
	clc
	adc #tile_size
	sta tmp2
	sta vram_VY

StartFirstDraw:
	;; Reset x to the starting x position for each row
	;; The $00 here is modified above to be the correct value
RowStartingXPosParam:
	ldx #$00

	;; The index into the tilemap
	;; The $00 here is modified above to be the correct value
	lda tmp1

	;; Increment the tilemap starting index by 32 pixels
	;; This is the width of a single row of tiles
	adc #tilemap_width
	;; Store the newly calculated value into the last location
	sta tmp1
	tay

DrawTile:
	;; Set our x pos
	stx vram_VX
	;; Increment x pos by tile size for the next draw
	txa
	clc
	adc #tile_size
	tax

	lda _tilemap,y
	sta vram_COLOR
	iny

	;; Actually start the draw!
	lda #$01
	sta vram_START

	;; Have we drawn enough x tiles to move to the next row?
	txa
	cmp #$80-tile_size
	bpl StartOfRow

	;; These nops are necessary to make sure the blit is finished before we start messing with it
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop


	bra DrawTile
End:
	jmp     incsp2
.endproc


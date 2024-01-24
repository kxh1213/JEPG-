CC = gcc

TARGET_ENCODER = encoder
TARGET_DECODER = decoder

SRCS_ENCODER = $(TARGET_ENCODER).c
SRCS_DECODER = $(TARGET_DECODER).c



all: $(TARGET_ENCODER) 
all: $(TARGET_DECODER)

$(TARGET_ENCODER): $(SRCS_ENCODER)
	$(CC) $(CFLAGS) -o $@ $(SRCS_ENCODER) -lm

$(TARGET_DECODER): $(SRCS_DECODER)
	$(CC) $(CFLAGS) -o $@ $(SRCS_DECODER) -lm

clean:
	rm -f $(TARGET_ENCODER) $(TARGET_DECODER) R.txt G.txt B.txt dim.txt ResKimberly.bmp

demo: demo

demo0: 
	./$(TARGET_ENCODER) 0 KimberlyNCat.bmp R.txt G.txt B.txt dim.txt
	./$(TARGET_DECODER) 0 ResKimberly.bmp R.txt G.txt B.txt dim.txt
	cmp <(tail -n +2 KimberlyNCat.bmp) <(tail -n +2 ResKimberly.bmp)



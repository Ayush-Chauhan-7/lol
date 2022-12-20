all: PartA PartC
PartA:
	gcc BonusAssignment_A.c -lpthread -o BonusAssignment_A
PartC:
	gcc BonusAssignment_C.c -lpthread -o BonusAssignment_C


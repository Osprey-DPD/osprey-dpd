dpd

Title	" Comb diblock copolymer  "
Date	10/02/21
Comment	"  Two component ring micelle   "


State   random


Bead	W
    	0.5
    	25	
    	4.5	

Bead	H1
    	0.5
    	100   25
    	4.5   4.5

Bead	T1
    	0.5
    	100   25   25
    	4.5   4.5  4.5

Bead	B
    	0.5
    	100   25   25    25
    	4.5   4.5  4.5   4.5

Bead	S
    	0.5
    	20   55   55    55    55
    	4.5  4.5  4.5   4.5   4.5

Bead	B1
    	0.5
    	100   25   25    25   55   25
    	4.5   4.5  4.5   4.5  4.5  4.5

Bead	S1
    	0.5
    	20   55   55    55    55   55   55
    	4.5  4.5  4.5   4.5   4.5  4.5  4.5


Bond	H1 B   128    0.5
Bond	T1 B   128    0.5
Bond	B  B   128    0.5
Bond	S  B   128    0.5
Bond	S  S   128    0.5

Bond	H1 B1  128    0.5
Bond	T1 B1  128    0.5
Bond	B1 B1  128    0.5
Bond	S1 B1  128    0.5
Bond	S1 S1  128    0.5


Polymer Water  0.9996   " (W) "
Polymer Comb1  0.0002   " (H1 (8 (B B B (* (S (6 S) S)) B B B)) T1) "
Polymer Comb2  0.0002   " (H1 (8 (B1 B1 B1 (* (S1 S1 S1 S1)) B1 B1 B1)) T1) "

Box		32  32  32			1 1 1
Density	3
Temp		1
RNGSeed	-7688
Lambda		0.5
Step		0.005
Time		1000000
SamplePeriod	100
AnalysisPeriod	500000
DensityPeriod	1000000
DisplayPeriod	5000
RestartPeriod	500000
Grid		1 1 1


Command	ToggleBeadDisplay      1   W
Command	SetCurrentStateCamera	1   0.5 -1.5 0.5  0.5 0.5 0.5

Command	SetTimeStepSize        1000  0.02






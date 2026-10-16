10 REM Compile BASIC to ARM32, then count down from five
20 LET A = 5
30 LET B = 1
40 LET C = 0
50 PRINT "Countdown from ARM32:\n"
60 PRINT A
70 LET A = A - B
80 IF A > C THEN GOTO 60
90 PRINT "Done.\n"
100 END

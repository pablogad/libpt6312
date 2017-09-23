v 20130925 2
C 40000 40000 0 0 0 title-B.sym
C 46000 47800 1 90 0 BC547-3.sym
{
T 45800 48800 5 10 0 1 180 0 1
device=BC547
T 46100 48100 5 10 1 1 180 0 1
refdes=Q1
T 45000 48700 5 10 0 1 90 0 1
footprint=TO92
}
C 46000 44300 1 90 0 BC547-3.sym
{
T 45800 45300 5 10 0 1 180 0 1
device=BC547
T 46100 44600 5 10 1 1 180 0 1
refdes=Q2
T 45000 45200 5 10 0 1 90 0 1
footprint=TO92
}
C 52900 47800 1 90 0 BC547-3.sym
{
T 52700 48800 5 10 0 1 180 0 1
device=BC547
T 52800 48000 5 10 1 1 180 0 1
refdes=Q3
T 51900 48700 5 10 0 1 90 0 1
footprint=TO92
}
C 51900 47800 1 270 0 BC547-3.sym
{
T 51600 46900 5 10 0 1 0 0 1
device=BC547
T 52600 46900 5 10 1 1 0 0 1
refdes=Q4
T 52900 46900 5 10 0 1 270 0 1
footprint=TO92
}
C 51500 47700 1 0 0 resistor-1.sym
{
T 51800 48100 5 10 0 0 0 0 1
device=RESISTOR
T 51500 47600 5 10 1 1 0 0 1
refdes=R8
T 51400 47400 5 10 1 1 0 0 1
value=10K
T 51500 47700 5 10 0 1 0 0 1
footprint=res_vert
}
C 52000 48400 1 90 0 resistor-1.sym
{
T 51600 48700 5 10 0 0 90 0 1
device=RESISTOR
T 51800 49100 5 10 1 1 180 0 1
refdes=R7
T 51400 48700 5 10 1 1 0 0 1
value=10K
T 52000 48400 5 10 0 1 0 0 1
footprint=res_vert
}
C 53000 48400 1 90 0 resistor-1.sym
{
T 52600 48700 5 10 0 0 90 0 1
device=RESISTOR
T 53300 49100 5 10 1 1 180 0 1
refdes=R9
T 53100 48700 5 10 1 1 0 0 1
value=10K
T 53000 48400 5 10 0 1 0 0 1
footprint=res_vert
}
C 45100 44900 1 90 0 resistor-1.sym
{
T 44700 45200 5 10 0 0 90 0 1
device=RESISTOR
T 44900 45600 5 10 1 1 180 0 1
refdes=R4
T 44500 45200 5 10 1 1 0 0 1
value=10K
T 45100 44900 5 10 0 1 0 0 1
footprint=res_vert
}
C 45400 44300 1 270 0 resistor-1.sym
{
T 45800 44000 5 10 0 0 270 0 1
device=RESISTOR
T 45100 43900 5 10 1 1 0 0 1
refdes=R6
T 45300 43800 5 10 1 1 180 0 1
value=10K
T 45400 44300 5 10 0 1 180 0 1
footprint=res_vert
}
C 46100 44900 1 90 0 resistor-1.sym
{
T 45700 45200 5 10 0 0 90 0 1
device=RESISTOR
T 46400 45600 5 10 1 1 180 0 1
refdes=R5
T 46200 45200 5 10 1 1 0 0 1
value=10K
T 46100 44900 5 10 0 1 0 0 1
footprint=res_vert
}
C 45400 47800 1 270 0 resistor-1.sym
{
T 45800 47500 5 10 0 0 270 0 1
device=RESISTOR
T 45100 47400 5 10 1 1 0 0 1
refdes=R3
T 45300 47300 5 10 1 1 180 0 1
value=10K
T 45400 47800 5 10 0 1 180 0 1
footprint=res_vert
}
C 45100 48400 1 90 0 resistor-1.sym
{
T 44700 48700 5 10 0 0 90 0 1
device=RESISTOR
T 44900 49100 5 10 1 1 180 0 1
refdes=R1
T 44500 48700 5 10 1 1 0 0 1
value=10K
T 45100 48400 5 10 0 0 0 0 1
footprint=res_vert
}
C 46100 48400 1 90 0 resistor-1.sym
{
T 45700 48700 5 10 0 0 90 0 1
device=RESISTOR
T 46400 49100 5 10 1 1 180 0 1
refdes=R2
T 46200 48700 5 10 1 1 0 0 1
value=10K
T 46100 48400 5 10 0 1 0 0 1
footprint=res_vert
}
N 51900 48400 51900 47200 4
N 52900 48400 52900 47200 4
N 51900 48400 51000 48400 4
{
T 50600 48400 5 10 1 1 0 0 1
pinlabel=DIO
T 51900 48400 5 10 0 0 0 0 1
netname=DIO
}
N 52900 48400 53700 48400 4
{
T 53800 48400 5 10 1 1 0 0 1
pinlabel=DATA_PT6312
T 52900 48400 5 10 0 0 0 0 1
netname=DATA_VFD
}
C 51700 49300 1 0 0 3.3V-plus-1.sym
C 51300 47800 1 0 0 3.3V-plus-1.sym
C 52700 49300 1 0 0 5V-plus-1.sym
C 45800 45800 1 0 0 5V-plus-1.sym
C 45800 49300 1 0 0 5V-plus-1.sym
C 44800 49300 1 0 0 3.3V-plus-1.sym
C 44800 45800 1 0 0 3.3V-plus-1.sym
C 45700 46900 1 0 0 3.3V-plus-1.sym
N 45500 46900 45900 46900 4
N 46000 48400 46700 48400 4
{
T 46800 48400 5 10 1 1 0 0 1
pinlabel=CS_PT6312
T 46000 48400 5 10 0 0 0 0 1
netname=CS_VFD
}
N 45000 48400 44300 48400 4
{
T 43800 48400 5 10 1 1 0 0 1
pinlabel=STB
T 45000 48400 5 10 0 0 0 0 1
netname=STB
}
C 45700 43400 1 0 0 3.3V-plus-1.sym
N 45500 43400 45900 43400 4
N 46000 44900 46700 44900 4
{
T 46800 44900 5 10 1 1 0 0 1
pinlabel=CLK_PT6312
T 46000 44900 5 10 0 0 0 0 1
netname=CLK_VFD
}
N 45000 44900 44300 44900 4
{
T 44100 45000 5 10 1 1 0 0 1
pinlabel=CLK
T 45000 44900 5 10 0 0 0 0 1
netname=CLK
}
T 50100 40800 9 10 1 0 0 0 1
3v3 to 5v interface
T 54000 40100 9 10 1 0 0 0 1
Pablo GAD
C 52100 44200 1 90 0 resistor-1.sym
{
T 51700 44500 5 10 0 0 90 0 1
device=RESISTOR
T 51800 44800 5 10 1 1 180 0 1
refdes=RI
T 52100 44700 5 10 1 1 0 0 1
value=10K
T 52100 44200 5 10 0 1 0 0 1
footprint=res_vert
}
C 51800 45100 1 0 0 3.3V-plus-1.sym
N 50700 44200 53400 44200 4
{
T 50900 44000 5 10 1 1 0 0 1
pinlabel=IR
T 50700 44200 5 10 0 0 0 0 1
netname=IR
}
T 53600 44200 9 10 1 0 0 0 1
IR_OPEN_COLLECTOR
T 49600 44200 9 10 1 0 0 0 1
GPIO_LIRC
C 40600 43700 1 0 0 connector6-1.sym
{
T 42400 45500 5 10 0 0 0 0 1
device=CONNECTOR_6
T 40700 45700 5 10 1 1 0 0 1
refdes=CONN_VFD_BOARD
T 40600 43700 5 10 0 0 0 0 1
footprint=JST_XH_6
}
N 42300 48300 42600 48300 4
{
T 42700 48300 5 10 1 1 0 0 1
pinlabel=STB
T 42300 48300 5 10 0 0 0 0 1
netname=STB
}
N 42600 48000 42300 48000 4
{
T 42700 48000 5 10 1 1 0 0 1
pinlabel=CLK
T 42600 48000 5 10 0 0 0 0 1
netname=CLK
}
N 42600 47700 42300 47700 4
{
T 42700 47700 5 10 1 1 0 0 1
pinlabel=DIO
T 42600 47700 5 10 0 0 0 0 1
netname=DIO
}
N 42600 47400 42300 47400 4
{
T 42700 47400 5 10 1 1 0 0 1
pinlabel=IR
T 42600 47400 5 10 0 0 0 0 1
netname=IR
}
C 42200 46800 1 0 0 gnd-1.sym
N 42300 45400 42600 45400 4
{
T 42700 45400 5 10 1 1 0 0 1
pinlabel=5v
}
N 42300 45100 43300 45100 4
{
T 42700 45100 5 10 1 1 0 0 1
pinlabel=GND
T 42300 45100 5 10 0 0 0 0 1
netname=GND
}
N 42300 44800 42600 44800 4
{
T 42700 44800 5 10 1 1 0 0 1
pinlabel=DATA_PT6312
T 42300 44800 5 10 0 0 0 0 1
netname=DATA_VFD
}
N 42300 44500 42600 44500 4
{
T 42700 44500 5 10 1 1 0 0 1
pinlabel=CLK_PT6312
T 42300 44500 5 10 0 0 0 0 1
netname=CLK_VFD
}
N 42300 44200 42600 44200 4
{
T 42700 44200 5 10 1 1 0 0 1
pinlabel=CS_PT6312
T 42300 44200 5 10 0 0 0 0 1
netname=CS_VFD
}
N 42300 43900 42600 43900 4
{
T 42700 43900 5 10 1 1 0 0 1
pinlabel=IR
T 42300 43900 5 10 0 0 0 0 1
netname=IR
}
C 43400 45400 1 180 0 gnd-1.sym
C 44200 46300 1 180 0 terminal-1.sym
{
T 43890 45550 5 10 0 0 180 0 1
device=terminal
T 43890 45700 5 10 0 0 180 0 1
footprint=JUMPER1
T 43950 46250 5 10 1 1 180 6 1
refdes=T1
}
N 42600 45400 42600 46200 4
N 42600 46200 43300 46200 4
T 43100 46200 9 10 1 0 0 0 2
Connect to +5v VFD power supply

C 42400 46200 1 0 0 5V-plus-1.sym
C 40600 46900 1 0 0 connector6-1.sym
{
T 42400 48700 5 10 0 0 0 0 1
device=CONNECTOR_6
T 40700 48900 5 10 1 1 0 0 1
refdes=CONN_RASPI
T 40600 46900 5 10 0 0 0 0 1
footprint=JUMPER6
}
C 42100 48600 1 0 0 3.3V-plus-1.sym

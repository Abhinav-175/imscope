# LIGO nds2 data stream to stdout

import nds2

A = nds2.iterate(
channels=["W1:PPC-PPOL_DC_OUT_DQ"],
hostname="192.168.187.5",
port=8088
)

for i in A:
  for j in i[0].data:  
    print(j)

